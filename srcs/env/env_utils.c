/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:42:45 by rabiner           #+#    #+#             */
/*   Updated: 2025/08/27 18:39:08 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
// defini ou modifie une variable d'environnement
char	**set_env(char **env, const char *arg)
{
	char	*key;
	char	**new_env;
	int		i;
	int		j;
	size_t	len;

	if (!arg || !ft_strchr(arg, '='))
		return (env);
	len = 0;
	while (arg[len] && arg[len] != '=')
		len++;
	key = ft_substr(arg, 0, len);
	if (!key)
		return (env);
	////////////////////////////////////// ok
	if (!env)
	{
		new_env = malloc(sizeof(char *) * 2);
		if (!new_env)
		{
			free(key);
			return (NULL);
		}
		new_env[0] = ft_strdup(arg);
		if (!new_env[0])
		{
			free(new_env);
			free(key);
			return (NULL);
		}
		new_env[1] = NULL;
		free(key);
		return (new_env);
	}
	/////////////////////////////////////// ok
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, len) && env[i][len] == '=')
		{
			char *new_entry = ft_strdup(arg);
			if (!new_entry)                  
			{
				free(key);                   
				return (env);                
			}
			free(env[i]);                    
			env[i] = new_entry;              
			free(key);
			return (env);
		}
		i++;
	}
	//////////////////////////////////////////////// env, key, arg, 
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
	{
		free(key);
		return (env);
	}
	j = 0;
	while (j < i)
	{
		new_env[j] = env[j];
		j++;
	}
	new_env[i] = ft_strdup(arg);
	if (!new_env[i])
	{
		free(new_env);
		free(key);
		return (env);
	}
	new_env[i + 1] = NULL;
	free(env);
	free(key);
	return (new_env);
}
*/

static char	**set_no_env(char *key, const char *arg)
{
	char	**new_env;

	new_env = malloc(sizeof(char *) * 2);
	if (!new_env)
	{
		free(key);
		return (NULL);
	}
	new_env[0] = ft_strdup(arg);
	if (!new_env[0])
	{
		free(new_env);
		free(key);
		return (NULL);
	}
	new_env[1] = NULL;
	free(key);
	return (new_env);
}

static int	add_entry(char **env, char *key, int len, const char *arg)
{
	char	*new_entry;
	int		i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, len) && env[i][len] == '=')
		{
			new_entry = ft_strdup(arg);
			if (!new_entry)
			{
				free(key);
				return (1);
			}
			free(env[i]);
			env[i] = new_entry;
			free(key);
			return (1);
		}
		i++;
	}
	return (0);
}

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
	new_env[i] = ft_strdup(arg);
	if (!new_env[i])
	{
		free(new_env);
		free(key);
		return (1);
	}
	return (0);
}

static char	**free_copy_env(char **env, char *key, const char *arg)
{
	char	**new_env;
	int		i;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
	{
		free(key);
		return (env);
	}
	if (free_copy_env_2(env, key, arg, new_env))
		return (env);
	new_env[i + 1] = NULL;
	free(env);
	free(key);
	return (new_env);
}

char	**set_env(char **env, const char *arg)
{
	int		len;
	char	*key;

	if (!arg || !ft_strchr(arg, '='))
		return (env);
	len = 0;
	while (arg[len] && arg[len] != '=')
		len++;
	key = ft_substr(arg, 0, len);
	if (!key)
		return (env);
	if (!env)
		return (set_no_env(key, arg));
	if (add_entry(env, key, len, arg))
		return (env);
	return (free_copy_env(env, key, arg));
}
