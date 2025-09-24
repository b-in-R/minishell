/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:42:45 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/12 00:45:47 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	env_replace(t_pool *pool, char **env,
		const char *key, const char *arg)
{
	char	*dup;
	size_t	len;
	int		i;

	len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, len) && env[i][len] == '=')
		{
			dup = pool_strdup_ctx(pool, arg);
			if (!dup)
				return (1);
			pool_free_ctx(pool, env[i]);
			env[i] = dup;
			return (0);
		}
		i++;
	}
	return (-1);
}

static int	env_extend(t_pool *pool, char ***ref, char **env,
		const char *arg, char *key)
{
	char	**new_env;
	char	*dup;
	int		size;
	int		i;

	size = 0;
	while (env[size])
		size++;
	new_env = pool_alloc_ctx(pool, sizeof(char *) * (size + 2));
	if (!new_env)
	{
		pool_free_ctx(pool, key);
		return (1);
	}
	i = 0;
	while (i < size)
	{
		new_env[i] = env[i];
		i++;
	}
	dup = pool_strdup_ctx(pool, arg);
	if (!dup)
	{
		pool_free_ctx(pool, new_env);
		pool_free_ctx(pool, key);
		return (1);
	}
	new_env[size] = dup;
	new_env[size + 1] = NULL;
	pool_free_ctx(pool, env);
	pool_free_ctx(pool, key);
	*ref = new_env;
	return (0);
}

static int	env_create(t_pool *pool, char ***ref, const char *arg, char *key)
{
	char	**new_env;

	new_env = pool_alloc_ctx(pool, sizeof(char *) * 2);
	if (!new_env)
	{
		pool_free_ctx(pool, key);
		return (1);
	}
	new_env[0] = pool_strdup_ctx(pool, arg);
	if (!new_env[0])
	{
		pool_free_ctx(pool, new_env);
		pool_free_ctx(pool, key);
		return (1);
	}
	new_env[1] = NULL;
	*ref = new_env;
	pool_free_ctx(pool, key);
	return (0);
}

// Inserts or replaces an environment assignment in the given array.
int	set_env(t_pool *pool, char ***env_ref, const char *arg)
{
	char	**env;
	char	*key;
	size_t	len;
	int		status;

	if (!pool || !env_ref || !arg || !ft_strchr(arg, '='))
		return (1);
	len = 0;
	while (arg[len] && arg[len] != '=')
		len++;
	key = pool_substr_ctx(pool, arg, 0, len);
	if (!key)
		return (1);
	env = *env_ref;
	if (!env)
		return (env_create(pool, env_ref, arg, key));
	status = env_replace(pool, env, key, arg);
	if (status == 0)
	{
		pool_free_ctx(pool, key);
		return (0);
	}
	if (status == 1)
	{
		pool_free_ctx(pool, key);
		return (1);
	}
	return (env_extend(pool, env_ref, env, arg, key));
}
