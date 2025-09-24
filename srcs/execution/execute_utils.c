/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 14:45:03 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/06 10:29:20 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Counts how many command nodes compose the current pipeline.
int	count_cmds(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

// Allocates and initialises fork bookkeeping before execution.
void	initialise_data(t_fork *data, t_cmd *cmd, t_expander *exp)
{
	data->fd[0] = -1;
	data->fd[1] = -1;
	data->in_fd = 0;
	data->pid = pool_alloc_ctx(exp->pool, sizeof(pid_t) * count_cmds(cmd));
	if (!data->pid)
		error_exit(exp->pool, exp->my_env, "execute: malloc data->pid fail");
	data->status = 0;
	data->last_status = 0;
}

// Waits for child processes and captures the pipeline exit status.
void	take_exit_code(int *i, int *j, t_fork *data)
{
	int	stat;

	stat = data->status;
	while (*j < *i)
	{
		if (data->pid[*j] > 0)
		{
			waitpid(data->pid[*j], &stat, 0);
			if (*j == *i - 1)
			{
				if (WIFEXITED(stat))
					data->last_status = WEXITSTATUS(stat);
				else if (WIFSIGNALED(stat))
				{
					if (WTERMSIG(stat) == SIGINT)
						write(1, "\n", 1);
					else if (WTERMSIG(stat) == SIGQUIT)
						write(2, "Quit (core dumped)\n", 19);
					data->last_status = 128 + WTERMSIG(stat);
				}
			}
		}
		(*j)++;
	}
}
