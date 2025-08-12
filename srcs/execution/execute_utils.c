/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 14:45:03 by rabiner           #+#    #+#             */
/*   Updated: 2025/08/12 14:46:31 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


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

void	initialise_data(t_fork *data, t_cmd *cmd)
{
	data->fd[0] = -1;
	data->fd[1] = -1;
	data->in_fd = 0;
	data->pid = malloc(sizeof(pid_t) * count_cmds(cmd));
	data->status = 0;
	data->last_status = 0;
}