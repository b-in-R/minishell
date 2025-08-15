/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:51:32 by rabiner           #+#    #+#             */
/*   Updated: 2025/08/15 18:35:27 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**init_env(char **envp)
{
	int		i;
	int		len;
	char	**copy;

	i = 0;
	while (envp[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return (NULL);
	len = 0;
	while (envp[len])
	{
		copy[len] = ft_strdup(envp[len]);
		if (!copy[len])
		{
			while (--len >= 0)
				free(copy[len]);
			free(copy);
			return (NULL);
		}
		len++;
	}
	copy[len] = NULL;
	return (copy);
}

void	free_env(char **my_env)
{
	int	i;

	if (!my_env)
		return;
	i = 0;
	while (my_env[i])
	{
		free(my_env[i]);
		my_env[i] = NULL;
		i++;
	}
	free(my_env);
}

void	print_env(char **env)
{
	int	i = 0;

	// trace env
	printf("DGB my_env@print = %p\n", (void *)env);
	fflush(stdout);// forcer l'affichage avant les write
	//
	
	while (env && env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

