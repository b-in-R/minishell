/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:19:42 by rabiner           #+#    #+#             */
/*   Updated: 2025/08/27 12:24:36 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	safe_close(int fd)
{
	if (fd > 2)
		close(fd);
}

static void	safe_dup2_close(int src, int dst)
{
	if (src >= 0 && src != dst)
	{
		if (dup2(src, dst) == -1)
			error_exit(NULL, "safe_dup2_close\n");
		safe_close(src);
	}
}

// configure stdin/stdout avant execution d'une commande selon
// pipe ou cmd unique
void	set_redirection(char **my_env, t_cmd *cmd, int in_fd, int pipe_fd[2])
{
	int	fd_in;
	int	flags;
	int	fd_out;

	if (cmd->in_fd != -1)
		safe_dup2_close(cmd->in_fd, STDIN_FILENO);
	else if (cmd->infile)
	{
		fd_in = open(cmd->infile, O_RDONLY);
		if (fd_in < 0)
			error_exit(my_env, "setup_redirections: open infile");
		safe_dup2_close(fd_in, STDIN_FILENO);
	}
	else if (in_fd >= 0 && in_fd != STDIN_FILENO)
		safe_dup2_close(in_fd, STDIN_FILENO);
	if (cmd->outfile)
	{
		flags = O_CREAT | O_WRONLY;
		if (cmd->append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		fd_out = open(cmd->outfile, flags, 0644);
		if (fd_out < 0)
			error_exit(my_env, "setup_redirections: open outfile");
		safe_dup2_close(fd_out, STDOUT_FILENO);
	}
	else if (cmd->next && pipe_fd[1] >= 0)
	{
		safe_dup2_close(pipe_fd[1], STDOUT_FILENO);
		pipe_fd[1] = -1;
	}
	if (pipe_fd[0] >= 0)
		safe_close(pipe_fd[0]);
	if (pipe_fd[1] >= 0)
		safe_close(pipe_fd[1]);
}
