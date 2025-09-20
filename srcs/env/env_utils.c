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

// Creates a fresh environment array when none exists yet.
static char	**set_no_env(char *key, const char *arg)// allocs
{
	char	**new_env;

	new_env = pool_alloc_ctx(sizeof(char *) * 2);
	if (!new_env)
	{
		pool_free_ctx(key);
		return (NULL);
	}
	new_env[0] = pool_strdup_ctx(arg);
	if (!new_env[0])
	{
		pool_free_ctx(new_env);
		pool_free_ctx(key);
		return (NULL);
	}
	new_env[1] = NULL;
	pool_free_ctx(key);
	return (new_env);
}

// Updates an existing variable if `key` already exists in env.
static int	add_entry(char **env, char *key, int len, const char *arg)
{
	char	*new_entry;
	int		i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, len) && env[i][len] == '=')
		{
		new_entry = pool_strdup_ctx(arg);
		if (!new_entry)
		{
			pool_free_ctx(key);
			return (1);
		}
		pool_free_ctx(env[i]);
		env[i] = new_entry;
		pool_free_ctx(key);
		return (1);
	}
		i++;
	}
	return (0);
}

// Copies pointers into the enlarged env array and appends the new entry.
int	free_copy_env_2(char **env, char *key, const char *arg, char **new_env)
{
	int	j;
	int	i;

	i = 0;
	j = 0;
	while (env[i])
		i++;
	while (j < i)
	{
		new_env[j] = env[j];
		j++;
	}
	new_env[i] = pool_strdup_ctx(arg);
	if (!new_env[i])
	{
		pool_free_ctx(new_env);
		pool_free_ctx(key);
		return (1);
	}
	return (0);
}

// Grows the environment array and duplicates the new assignment.
static char	**free_copy_env(char **env, char *key, const char *arg)
{
	char	**new_env;
	int		i;

	i = 0;
	while (env[i])
		i++;
	new_env = pool_alloc_ctx(sizeof(char *) * (i + 2));
	if (!new_env)
	{
		pool_free_ctx(key);
		return (env);
	}
	if (free_copy_env_2(env, key, arg, new_env))
		return (env);
	new_env[i + 1] = NULL;
	pool_free_ctx(env);
	pool_free_ctx(key);
	return (new_env);
}

// Inserts or replaces an environment assignment in the given array.
char	**set_env(char **env, const char *arg)
{
	int		len;
	char	*key;

	if (!arg || !ft_strchr(arg, '='))
		return (env);
	len = 0;
	while (arg[len] && arg[len] != '=')
		len++;
	key = pool_substr_ctx(arg, 0, len);
	if (!key)
		return (env);
	if (!env)
		return (set_no_env(key, arg));// return ((char **)new_env);
	if (add_entry(env, key, len, arg))// -> malloc
		return (env);
	return (free_copy_env(env, key, arg));// return ((char **)new_env);
}
