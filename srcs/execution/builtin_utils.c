/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:42:45 by rabiner           #+#    #+#             */
/*   Updated: 2025/06/03 18:57:30 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

char	*get_env(char *str)
{
	int		i;
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	i = 0;
	while (g_env && g_env[i])
	{
		if (!ft_strncmp(g_env[i], str, len) && g_env[i][len] == '=')
			return (g_env[i] + len  + 1);
		i++;
	}
	return (NULL);
}

int	set_env(char *arg)
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
	while (g_env && g_env[i])
	{
		if (!ft_strncmp(g_env[i], key, len) && g_env[i][len] == '=')
		{
			free(g_env[i]);
			g_env[i] = ft_strdup(arg);
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
		new_env[j] = g_env[j];
		j++;
	}
	new_env[i] = ft_strdup(arg);
	new_env[i + 1] = NULL;
	free(g_env);
	g_env = new_env;
	free(key);
	return (0);
}

void	unset_env(char *arg)// explications
{
	int		i;
	int		j;
	size_t	len;
	
	if (!arg || !g_env)
		return ;
	len = ft_strlen(arg);
	i = 0;
	while (g_env[i])
	{
		if (!ft_strncmp(g_env[i], arg, len) && g_env[i][len] == '=')
		{
			free(g_env[i]);
			j = i;
			while (g_env[j + 1])
			{
				g_env[j] = g_env[j + 1];
				j++;
			}
			g_env[j] = NULL;
			return ;
		}
		i++;
	}
}
