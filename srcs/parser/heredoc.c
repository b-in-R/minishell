/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 09:55:28 by albertooutu       #+#    #+#             */
/*   Updated: 2025/09/23 13:37:41 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <errno.h>

int	append_char(t_pool *pool, char **str, char c);
int	handle_dollar(t_pool *pool, const char *str, int *i,
		char **result, t_expander *exp);

/*
** Ferme le FD du pipe, nettoie le pool et termine le fils avec le statut donné.
*/
static void	heredoc_child_exit(t_expander *exp, int write_fd, int status)
{
	pool_close_ctx(exp->pool, write_fd);
	pool_cleanup_ctx(exp->pool);
	exit(status);
}

/*
** Assure l'écriture totale d'un tampon en gérant les interruptions système.
*/
static int	write_all(int fd, const char *buf, size_t len)
{
	size_t	total;
	ssize_t	written;

	total = 0;
	while (total < len)
	{
		written = write(fd, buf + total, len - total);
		if (written == -1)
		{
			if (errno == EINTR)
				continue ;
			return (0);
		}
		total += (size_t)written;
	}
	return (1);
}

/*
** Reconstitue une ligne en expandant les variables $ tout en préservant le reste.
*/
static char	*expand_heredoc_line(const char *line, t_expander *exp)
{
	char	*expanded;
	int		i;

	i = 0;
	expanded = pool_strdup_ctx(exp->pool, "");
	if (!expanded)
		return (NULL);
	while (line[i])
	{
		if (line[i] == '$')
		{
			if (!handle_dollar(exp->pool, line, &i, &expanded, exp))
				return (pool_free_ctx(exp->pool, expanded), NULL);
		}
		else if (!append_char(exp->pool, &expanded, line[i++]))
			return (pool_free_ctx(exp->pool, expanded), NULL);
	}
	return (expanded);
}

/*
** En cas d'échec du pool_track, libère la ligne et quitte proprement le fils.
*/
static void	handle_pool_track_failure(char *line, t_expander *exp, int write_fd)
{
	if (line)
		free(line);
	perror("minishell: heredoc track");
	heredoc_child_exit(exp, write_fd, 1);
}

/*
** Écrit la ligne (éventuellement expansée) dans le pipe en signalant tout échec.
*/
static int	push_line(int write_fd, char *line, t_expander *exp, t_cmd *cmd)
{
	char	*expanded;
	int		ok;

	ok = 1;
	if (cmd->expand_heredoc)
		expanded = expand_heredoc_line(line, exp);
	else
		expanded = pool_strdup_ctx(exp->pool, line);
	if (!expanded)
	{
		ft_putstr_fd("minishell: heredoc: allocation failed\n", 2);
		return (0);
	}
	if (!write_all(write_fd, expanded, ft_strlen(expanded))
		|| !write_all(write_fd, "\n", 1))
	{
		perror("minishell: heredoc write");
		ok = 0;
	}
	pool_free_ctx(exp->pool, expanded);
	return (ok);
}

/*
** Gère la boucle readline du heredoc: signaux, expansions et écriture sécurisée.
*/
static void	heredoc_child_process(int pipe_fd[2], t_cmd *cmd, t_expander *exp)
{
	char	*line;

	signal(SIGINT, sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
	pool_close_ctx(exp->pool, pipe_fd[0]);
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
		if (!pool_track_ctx(exp->pool, line))
			handle_pool_track_failure(line, exp, pipe_fd[1]);
		if (ft_strcmp(line, cmd->delimiter) == 0)
		{
			pool_free_ctx(exp->pool, line);
			heredoc_child_exit(exp, pipe_fd[1], 0);
		}
		if (!push_line(pipe_fd[1], line, exp, cmd))
		{
			pool_free_ctx(exp->pool, line);
			heredoc_child_exit(exp, pipe_fd[1], 1);
		}
		pool_free_ctx(exp->pool, line);
	}
}

/*
** Attend le fils heredoc, traduit son statut et restaure les handlers parents.
*/
static int	heredoc_parent_process(int pipe_fd[2], pid_t pid, t_cmd *cmd,
				t_expander *exp)
{
	int	status;
	pid_t	wait_rc;
	int		exit_status;

	wait_rc = waitpid(pid, &status, 0);
	pool_close_ctx(exp->pool, pipe_fd[1]);
	if (wait_rc == -1)
	{
		perror("minishell: waitpid");
		pool_close_ctx(exp->pool, pipe_fd[0]);
		cmd->in_fd = -1;
		exp->last_status = 1;
		setup_signals();
		return (0);
	}
	if (WIFSIGNALED(status))
	{
		pool_close_ctx(exp->pool, pipe_fd[0]);
		cmd->in_fd = -1;
		exp->last_status = 128 + WTERMSIG(status);
		setup_signals();
		return (0);
	}
	if (!WIFEXITED(status))
	{
		pool_close_ctx(exp->pool, pipe_fd[0]);
		cmd->in_fd = -1;
		exp->last_status = 1;
		setup_signals();
		return (0);
	}
	exit_status = WEXITSTATUS(status);
	if (exit_status != 0)
	{
		pool_close_ctx(exp->pool, pipe_fd[0]);
		cmd->in_fd = -1;
		exp->last_status = exit_status;
		setup_signals();
		return (0);
	}
	cmd->in_fd = pipe_fd[0];
	exp->last_status = 0;
	setup_signals();
	return (1);
}

/*
** Enregistre les deux extrémités du pipe dans le pool, sinon referme et échoue.
*/
static int	register_pipe_fds(t_expander *exp, int pipe_fd[2])
{
	if (!pool_track_fd(exp->pool, pipe_fd[0]))
	{
		ft_putstr_fd("minishell: heredoc: fd tracking failed\n", 2);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (0);
	}
	if (!pool_track_fd(exp->pool, pipe_fd[1]))
	{
		ft_putstr_fd("minishell: heredoc: fd tracking failed\n", 2);
		pool_close_ctx(exp->pool, pipe_fd[0]);
		close(pipe_fd[1]);
		return (0);
	}
	return (1);
}

/*
** Prépare un heredoc: crée le pipe, fork, gère les erreurs et transmet l'in_fd.
*/
static int	process_single_heredoc(t_cmd *cmd, t_expander *exp)
{
	int		pipe_fd[2];
	pid_t	pid;

	cmd->in_fd = -1;
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		exp->last_status = 1;
		return (0);
	}
	if (!register_pipe_fds(exp, pipe_fd))
	{
		exp->last_status = 1;
		return (0);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		pool_close_ctx(exp->pool, pipe_fd[0]);
		pool_close_ctx(exp->pool, pipe_fd[1]);
		exp->last_status = 1;
		setup_signals();
		return (0);
	}
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
