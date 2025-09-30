/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_p_process_ifs.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 19:15:29 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/27 20:53:37 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	heredoc_wait_rc(t_cmd *cmd, t_expander *exp, int pipe_fd[2])
{
	perror("minishell: waitpid");
	pool_close_one(exp->pool, pipe_fd[0]);
	cmd->in_fd = -1;
	exp->last_status = 1;
	setup_signals();
	return (0);
}

int	heredoc_wifsignaled(t_cmd *cmd, t_expander *exp, int pipe_fd[2],
		int status)
{
	pool_close_one(exp->pool, pipe_fd[0]);
	cmd->in_fd = -1;
	exp->last_status = 128 + WTERMSIG(status);
	setup_signals();
	return (0);
}

int	heredoc_wifexited(t_cmd *cmd, t_expander *exp, int pipe_fd[2])
{
	pool_close_one(exp->pool, pipe_fd[0]);
	cmd->in_fd = -1;
	exp->last_status = 1;
	setup_signals();
	return (0);
}

int	heredoc_exit(t_cmd *cmd, t_expander *exp, int pipe_fd[2], int status)
{
	pool_close_one(exp->pool, pipe_fd[0]);
	cmd->in_fd = -1;
	exp->last_status = WEXITSTATUS(status);
	setup_signals();
	return (0);
}
