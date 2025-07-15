/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 09:55:28 by albertooutu       #+#    #+#             */
/*   Updated: 2025/07/14 10:19:09 by albertooutu      ###   ########.fr       */
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

int	handle_heredocs(t_cmd *cmds, t_expander *exp)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;
	char	*line;
	char	*expanded_line;

	while (cmds)
	{
		if (cmds->heredoc && cmds->delimiter)
		{
			if (pipe(pipe_fd) == -1)
				return (perror("pipe"), 0);
			pid = fork();
			if (pid == -1)
				return (perror("fork"), 0);
			if (pid == 0)
			{
				signal(SIGINT, SIG_DFL);
				while (1)
				{
					line = readline("> ");
					if (!line)
						exit(0);
					if (ft_strcmp(line, cmds->delimiter) == 0)
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
			else
			{
				waitpid(pid, &status, 0);
				close(pipe_fd[1]);
				if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
				{
					close(pipe_fd[0]);
					cmds->in_fd = -1;
					return (0);
				}
				cmds->in_fd = pipe_fd[0];
			}
		}
		cmds = cmds->next;
	}
	return (1);
}
