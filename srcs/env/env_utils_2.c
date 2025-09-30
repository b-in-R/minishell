/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:37:51 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/28 19:07:27 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// retrouver la valeur d'une variable d'environnement (aussi avant execve)
char	*get_env(char **my_env, char *str)
{
	int		i;
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	i = 0;
	while (my_env && my_env[i])
	{
		if (!ft_strncmp(my_env[i], str, len) && my_env[i][len] == '=')
			return (my_env[i] + len + 1);
		i++;
	}
	return (NULL);
}

// supprime une variable d'environnement
void	unset_env(t_pool *pool, char **my_env, char *arg)
{
	int		i;
	int		j;
	size_t	len;

	if (!arg || !my_env)
		return ;
	len = ft_strlen(arg);
	i = 0;
	while (my_env[i])
	{
		if (!ft_strncmp(my_env[i], arg, len) && my_env[i][len] == '=')
		{
			pool_free_one(pool, my_env[i]);
			j = i;
			while (my_env[j + 1])
			{
				my_env[j] = my_env[j + 1];
				j++;
			}
			my_env[j] = NULL;
			return ;
		}
		i++;
	}
}

// Removes an entry from env when it matches `key` (before '=').
int	remove_from_env(t_pool *pool, char **env, const char *key)
{
	int		i;
	int		j;
	size_t	var_name_len;

	var_name_len = 0;
	while (key[var_name_len] && key[var_name_len] != '=')
		var_name_len++;
	i = 0;
	while (env && env[i])
	{
		if (ft_strncmp(env[i], key, var_name_len) == 0
			&& env[i][var_name_len] == '=')
		{
			pool_free_one(pool, env[i]);
			j = i;
			while (env[j])
			{
				env[j] = env[j + 1];
				j++;
			}
			return (1);
		}
		i++;
	}
	return (0);
}

/*
*	Checks if a string is a valid identifier for an environment variable
*/
int	is_valid_identifier(const char *str)
{
	int	i;
	int	len;

	if (!str || !*str)
		return (0);
	len = 0;
	while (str[len] && str[len] != '=')
		len++;
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (i < len)
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	env_replace(t_pool *pool, char **env, const char *key, const char *arg)
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
			dup = pool_strdup(pool, arg);
			if (!dup)
				return (1);
			pool_free_one(pool, env[i]);
			env[i] = dup;
			return (0);
		}
		i++;
	}
	return (-1);
}
