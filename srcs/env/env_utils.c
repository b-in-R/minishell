/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:42:45 by rabiner           #+#    #+#             */
/*   Updated: 2025/08/15 16:27:43 by rabiner          ###   ########.fr       */
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
			return (my_env[i] + len  + 1);
		i++;
	}
	return (NULL);
}

// defini ou modifie une variable d'environnement
char **set_env(char **env, const char *arg)
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

// supprime une variable d'environnement
void	unset_env(char **my_env, char *arg)
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
			free(my_env[i]);
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

/*
*	Removes a variable from the environment.
*	To move one variable from local_env to my_env
*/
int	remove_from_env(char **env, const char *key)
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
		if (ft_strncmp(env[i], key, var_name_len) == 0 && env[i][var_name_len] == '=')
		{
			free(env[i]);
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
