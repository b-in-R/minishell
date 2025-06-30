/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:09:09 by rabiner           #+#    #+#             */
/*   Updated: 2025/06/25 18:20:05 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleanup_parent(t_cmd *cmd, int *in_fd, int *fd)
{
	if (*in_fd != 0)
		close(*in_fd);
	if (cmd->next)
	{
		close(fd[1]);
		*in_fd = fd[0];
	}
	else
	{
		close(fd[0]);
		close(fd[1]);
	}
}



void	error_exit(char **my_env, char *str)
{
	printf(RED"ERROR_EXIT: %s\n"RST, str);
	free_env(my_env);
	exit(EXIT_FAILURE);
}