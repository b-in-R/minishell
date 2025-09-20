/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 09:55:28 by albertooutu       #+#    #+#             */
/*   Updated: 2025/09/20 00:30:23 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_variables(const char *line, t_expander *exp)
{
	t_token	*tokens;
	char	*result;

	tokens = lexer((char *)line);
	if (!tokens)
		return (pool_strdup_ctx(""));
	expand_tokens(tokens, exp);
	result = join_tokens(tokens);
	free_tokens(tokens);
	return (result);
}

static void	heredoc_child_process(int pipe_fd[2], t_cmd *cmd, t_expander *exp)
{
	char	*line;
	char	*expanded_line;

	signal(SIGINT, SIG_DFL);
	pool_close_ctx(pipe_fd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			pool_cleanup_ctx();
			exit(0);
		}
		pool_track_ctx(line);
		if (ft_strcmp(line, cmd->delimiter) == 0)
		{
			pool_free_ctx(line);
			pool_cleanup_ctx();
			exit(0);
		}
		expanded_line = expand_variables(line, exp);
		write(pipe_fd[1], expanded_line, ft_strlen(expanded_line));
		write(pipe_fd[1], "\n", 1);
		pool_free_ctx(line);
		pool_free_ctx(expanded_line);
	}
}

static int	heredoc_parent_process(int pipe_fd[2], pid_t pid, t_cmd *cmd)
{
	int	status;

	waitpid(pid, &status, 0);
	pool_close_ctx(pipe_fd[1]);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		pool_close_ctx(pipe_fd[0]);
		cmd->in_fd = -1;
		return (0);
	}
	cmd->in_fd = pipe_fd[0];
	return (1);
}

static int	process_single_heredoc(t_cmd *cmd, t_expander *exp)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), 0);
	pool_track_fd(pool_get_context(), pipe_fd[0]);
	pool_track_fd(pool_get_context(), pipe_fd[1]);
	pid = fork();
	if (pid == -1)
	{
		pool_close_ctx(pipe_fd[0]);
		pool_close_ctx(pipe_fd[1]);
		return (perror("fork"), 0);
	}
	if (pid == 0)
		heredoc_child_process(pipe_fd, cmd, exp);
	else
		return (heredoc_parent_process(pipe_fd, pid, cmd));
	return (1);
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
