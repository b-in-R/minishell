/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:42:45 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/28 19:07:42 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	copy_env_entries(char **dst, char **src, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		dst[i] = src[i];
		i++;
	}
}

static int	env_extend(t_pool *pool, char ***ref, char **env, const char *arg)
{
	char	**new_env;
	char	*dup;
	int		size;

	size = 0;
	while (env[size])
		size++;
	new_env = pool_alloc(pool, sizeof(char *) * (size + 2));
	if (!new_env)
		return (1);
	copy_env_entries(new_env, env, size);
	dup = pool_strdup(pool, arg);
	if (!dup)
	{
		pool_free_one(pool, new_env);
		return (1);
	}
	new_env[size] = dup;
	new_env[size + 1] = NULL;
	pool_free_one(pool, env);
	*ref = new_env;
	return (0);
}

static int	env_create(t_pool *pool, char ***ref, const char *arg, char *key)
{
	char	**new_env;

	new_env = pool_alloc(pool, sizeof(char *) * 2);
	if (!new_env)
	{
		pool_free_one(pool, key);
		return (1);
	}
	new_env[0] = pool_strdup(pool, arg);
	if (!new_env[0])
	{
		pool_free_one(pool, new_env);
		pool_free_one(pool, key);
		return (1);
	}
	new_env[1] = NULL;
	*ref = new_env;
	pool_free_one(pool, key);
	return (0);
}

static int	set_env_bis(t_pool *pool, char ***env_ref,
		const char *arg, char *key)
{
	char	**env;
	int		status;

	env = *env_ref;
	if (!env)
		return (env_create(pool, env_ref, arg, key));
	status = env_replace(pool, env, key, arg);
	if (status == 0)
	{
		pool_free_one(pool, key);
		return (0);
	}
	if (status == 1)
	{
		pool_free_one(pool, key);
		return (1);
	}
	status = env_extend(pool, env_ref, env, arg);
	pool_free_one(pool, key);
	return (status);
}

// Inserts or replaces an environment assignment in the given array.
int	set_env(t_pool *pool, char ***env_ref, const char *arg)
{
	char	*key;
	size_t	len;

	if (!pool || !env_ref || !arg || !ft_strchr(arg, '='))
		return (1);
	len = 0;
	while (arg[len] && arg[len] != '=')
		len++;
	key = pool_substr(pool, arg, 0, len);
	if (!key)
		return (1);
	return (set_env_bis(pool, env_ref, arg, key));
}
