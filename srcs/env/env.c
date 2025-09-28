/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:51:32 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/12 01:53:05 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
char	**init_env(char **envp, t_pool *global)
{
	int		i;
	int		len;
	char	**copy;

	i = 0;
	while (envp[i])
		i++;
	//copy = malloc(sizeof(char *) * (i + 1));
	copy = (char **)pool_alloc(global, sizeof(char *) * (i + 1));
	if (!copy)
		return (NULL);
	len = 0;
	while (envp[len])
	{
		copy[len] = ft_strdup(envp[len]);// malloc
		pool_track(global, copy[len]);
		if (!copy[len])
		{
			while (--len >= 0)
				//free(copy[len]);
				pool_free_one(global, copy[len]);
			//free(copy);
			pool_free_one(global, copy);
			return (NULL);
		}
		len++;
	}
	copy[len] = NULL;
	return (copy);
}
*/

// Releases partially duplicated environment entries after a failure.
static char	**env_rollback(char **copy, int filled, t_pool *global)
{
	int	j;

	j = filled - 1;
	while (j >= 0)
	{
		pool_free_one(global, copy[j]);
		j--;
	}
	pool_free_one(global, copy);
	return (NULL);
}

// Duplicates the inherited environment into the pool-managed storage.
char	**init_env(char **envp, t_pool *global)
{
	int		n;
	int		i;
	char	**copy;

	n = 0;
	i = 0;
	copy = NULL;
	while (envp[n] != NULL)
		n++;
	copy = (char **)pool_alloc(global, (size_t)(n + 1) * sizeof(char *));
	if (copy == NULL)
		return (NULL);
	while (i < n)
	{
		copy[i] = pool_strdup(global, envp[i]);
		if (copy[i] == NULL)
			return (env_rollback(copy, i, global));
		i++;
	}
	copy[n] = NULL;
	return (copy);
}

// Normalement plus besoin
// Frees every string tracked in the current context environment.
void	free_env(t_pool *pool, char **my_env)
{
	int	i;

	if (!my_env)
		return ;
	i = 0;
	while (my_env[i])
	{
		pool_free_one(pool, my_env[i]);
		my_env[i] = NULL;
		i++;
	}
	pool_free_one(pool, my_env);
}

// Prints each environment entry on its own line.
void	print_env(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}
