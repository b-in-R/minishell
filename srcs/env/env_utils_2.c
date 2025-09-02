/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:37:51 by rabiner           #+#    #+#             */
/*   Updated: 2025/08/27 18:39:17 by rabiner          ###   ########.fr       */
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
		if (ft_strncmp(env[i], key, var_name_len) == 0
			&& env[i][var_name_len] == '=')
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
