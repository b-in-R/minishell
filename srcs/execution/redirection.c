/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:19:42 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/09 11:19:20 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Closes tracked file descriptors while preserving stdio.
static void	safe_close(t_pool *pool, int fd)
{
	if (fd > 2)
		pool_close_ctx(pool, fd);
}

// Duplicates src onto dst then closes the original descriptor if needed.
static void	safe_dup2_close(t_pool *pool, int src, int dst)
{
	if (src >= 0 && src != dst)
	{
		if (dup2(src, dst) == -1)
			error_exit(pool, NULL, "safe_dup2_close");
		safe_close(pool, src);
	}
}

// Sets stdout to outfile or pipe write end then closes descriptors.
void	set_redirection_2(t_expander *exp, t_cmd *cmd, int pipe_fd[2])
{
	int	flags;
	int	fd_out;

	if (cmd->outfile)
	{
		flags = O_CREAT | O_WRONLY;
		if (cmd->append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		fd_out = pool_open_ctx(exp->pool, cmd->outfile, flags, 0644);
		if (fd_out < 0)
			error_exit(exp->pool, exp->my_env,
				"setup_redirection :open outfile");
		safe_dup2_close(exp->pool, fd_out, STDOUT_FILENO);
	}
	else if (cmd->next && pipe_fd[1] >= 0)
	{
		safe_dup2_close(exp->pool, pipe_fd[1], STDOUT_FILENO);
		pipe_fd[1] = -1;
	}
	if (pipe_fd[0] >= 0)
		safe_close(exp->pool, pipe_fd[0]);
	if (pipe_fd[1] >= 0)
		safe_close(exp->pool, pipe_fd[1]);
}

// Sets up input redirections, then delegates to handle outputs/pipe ends.
void	set_redirection(t_expander *exp, t_cmd *cmd, int in_fd, int pipe_fd[2])
{
	int	fd_in;

	fd_in = 0;
	if (cmd->in_fd != -1)
		safe_dup2_close(exp->pool, cmd->in_fd, STDIN_FILENO);
	else if (cmd->infile)
	{
		fd_in = pool_open_ctx(exp->pool, cmd->infile, O_RDONLY, 0);
		if (fd_in < 0)
			error_exit(exp->pool, exp->my_env,
				"setup_redirection: open infile");
		safe_dup2_close(exp->pool, fd_in, STDIN_FILENO);
	}
	else if (in_fd >= 0 && in_fd != STDIN_FILENO)
		safe_dup2_close(exp->pool, in_fd, STDIN_FILENO);
	set_redirection_2(exp, cmd, pipe_fd);
}
