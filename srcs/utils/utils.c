/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:09:09 by rabiner           #+#    #+#             */
/*   Updated: 2025/06/03 18:42:48 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

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

void	free_env(void)
{
	int	i;

	if (!g_env)
		return ;
	i = 0;
	while (g_env[i])
	{
		free(g_env[i]);
		i++;
	}
	free(g_env);
	g_env = NULL;
}

void	init_env(char **envp)
{
	int		i;
	int		len;

	i = 0;
	while (envp[i])
		i++;
	g_env = malloc(sizeof(char *) * (i + 1));
	if (!g_env)
		return ;
	len = 0;
	while (envp[len])
	{
		g_env[len] = ft_strdup(envp[len]);
		if (!g_env[len])
		{
			while (--len >= 0)
				free(g_env[len]);
			free(g_env);
			g_env = NULL;
			return ;
		}
		len++;
	}
	g_env[len] = NULL;
}

void	error_exit(char *str)
{
	printf("ERROR: %s\n", str);
	free_env();
	exit(EXIT_FAILURE);
}