/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:42:45 by rabiner           #+#    #+#             */
/*   Updated: 2025/06/25 18:35:09 by rabiner          ###   ########.fr       */
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
int	set_env(char **my_env, char *arg)
{
	char	*key;
	char	**new_env;
	int		i;
	int		j;
	size_t	len;

	if (!arg || ft_strchr(arg, '='))
		return (-1);

	len = 0;
	while (arg[len] && arg[len] != '=')
		len++;
	key = ft_substr(arg, 0, len);
	if (!key)
		return (-1);
	
	// verif si la variable existe deja
	i = 0;
	while (my_env && my_env[i])
	{
		if (!ft_strncmp(my_env[i], key, len) && my_env[i][len] == '=')
		{
			free(my_env[i]);
			my_env[i] = ft_strdup(arg);
			free(key);
			return (0);
		}
		i++;
	}

	// ajoute la variable a la fin
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (free(key), -1);
	j = 0;
	while (j < i)
	{
		new_env[j] = my_env[j];
		j++;
	}
	new_env[i] = ft_strdup(arg);
	new_env[i + 1] = NULL;
	free(my_env);
	my_env = new_env;
	free(key);
	return (0);
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
