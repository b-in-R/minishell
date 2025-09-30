/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 09:55:28 by albertooutu       #+#    #+#             */
/*   Updated: 2025/09/28 19:29:23 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	process_heredoc_line(int pipe_fd[2], t_cmd *cmd,
				t_expander *exp, char *line)
{
	if (!pool_track(exp->pool, line))
		handle_pool_track_failure(line, exp, pipe_fd[1]);
	if (ft_strcmp(line, cmd->delimiter) == 0)
	{
		pool_free_one(exp->pool, line);
		heredoc_child_exit(exp, pipe_fd[1], 0);
	}
	if (!push_line(pipe_fd[1], line, exp, cmd))
	{
		pool_free_one(exp->pool, line);
		heredoc_child_exit(exp, pipe_fd[1], 1);
	}
	pool_free_one(exp->pool, line);
}

/*
** Gère la boucle readline du heredoc: signaux, expansions et écriture sécurisée.
*/
static void	heredoc_child_process(int pipe_fd[2], t_cmd *cmd, t_expander *exp)
{
	char	*line;

	signal(SIGINT, sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
	pool_close_one(exp->pool, pipe_fd[0]);
	while (1)
	{
		g_signal = 0;
		line = readline("> ");
		if (!line)
		{
			if (g_signal == SIGINT)
			{
				g_signal = 0;
				heredoc_child_exit(exp, pipe_fd[1], 130);
			}
			heredoc_child_exit(exp, pipe_fd[1], 0);
		}
		process_heredoc_line(pipe_fd, cmd, exp, line);
	}
}

/*
** Attend le fils heredoc, traduit son statut et restaure les handlers parents.
*/
static int	heredoc_parent_process(int pipe_fd[2], pid_t pid, t_cmd *cmd,
				t_expander *exp)
{
	int		status;
	pid_t	wait_rc;

	wait_rc = waitpid(pid, &status, 0);
	pool_close_one(exp->pool, pipe_fd[1]);
	if (wait_rc == -1)
		return (heredoc_wait_rc(cmd, exp, pipe_fd));
	if (WIFSIGNALED(status))
		return (heredoc_wifsignaled(cmd, exp, pipe_fd, status));
	if (!WIFEXITED(status))
		return (heredoc_wifexited(cmd, exp, pipe_fd));
	if (WEXITSTATUS(status))
		return (heredoc_exit(cmd, exp, pipe_fd, status));
	cmd->in_fd = pipe_fd[0];
	exp->last_status = 0;
	setup_signals();
	return (1);
}

/*
** Prépare un heredoc: crée le pipe, fork, gère les erreurs et transmet l'in_fd.
*/
static int	process_single_heredoc(t_cmd *cmd, t_expander *exp)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (!init_heredoc_pipe(cmd, exp, pipe_fd))
		return (0);
	if (!track_heredoc_fds(exp, pipe_fd))
		return (0);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		return (heredoc_fork_error(exp, pipe_fd));
	if (pid == 0)
		heredoc_child_process(pipe_fd, cmd, exp);
	return (heredoc_parent_process(pipe_fd, pid, cmd, exp));
}

/*
* Version refactorisée de handle_heredocs (maintenant 15 lignes)
*/
int	handle_heredocs(t_cmd *cmds, t_expander *exp)
{
	while (cmds)
	{
		if (cmds->heredoc && cmds->delimiter)
		{
			if (!process_single_heredoc(cmds, exp))
				return (0);
		}
		cmds = cmds->next;
	}
	return (1);
}
