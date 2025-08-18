/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 09:55:28 by albertooutu       #+#    #+#             */
/*   Updated: 2025/08/18 10:57:22 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_variables(const char *line, t_expander *exp)
{
	t_token	*tokens;
	char	*result;

	tokens = lexer((char *)line);
	if (!tokens)
		return (ft_strdup(""));
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
	close(pipe_fd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line)
			exit(0);
		if (ft_strcmp(line, cmd->delimiter) == 0)
		{
			free(line);
			exit(0);
		}
		expanded_line = expand_variables(line, exp);
		write(pipe_fd[1], expanded_line, ft_strlen(expanded_line));
		write(pipe_fd[1], "\n", 1);
		free(line);
		free(expanded_line);
	}
}

static int	heredoc_parent_process(int pipe_fd[2], pid_t pid, t_cmd *cmd)
{
	int	status;

	waitpid(pid, &status, 0);
	close(pipe_fd[1]);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipe_fd[0]);
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
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 0);
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
