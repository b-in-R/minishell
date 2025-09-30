/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 16:53:52 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/28 16:56:29 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	track_heredoc_fds(t_expander *exp, int pipe_fd[2])
{
	if (!pool_track_fd(exp->pool, pipe_fd[0]))
	{
		ft_putstr_fd("minishell: heredoc: fd tracking failed\n", 2);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		exp->last_status = 1;
		return (0);
	}
	if (!pool_track_fd(exp->pool, pipe_fd[1]))
	{
		ft_putstr_fd("minishell: heredoc: fd tracking failed\n", 2);
		pool_close_one(exp->pool, pipe_fd[0]);
		close(pipe_fd[1]);
		exp->last_status = 1;
		return (0);
	}
	return (1);
}

int	heredoc_fork_error(t_expander *exp, int pipe_fd[2])
{
	perror("minishell: fork");
	pool_close_one(exp->pool, pipe_fd[0]);
	pool_close_one(exp->pool, pipe_fd[1]);
	exp->last_status = 1;
	setup_signals();
	return (0);
}

int	init_heredoc_pipe(t_cmd *cmd, t_expander *exp, int pipe_fd[2])
{
	cmd->in_fd = -1;
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		exp->last_status = 1;
		return (0);
	}
	return (1);
}
