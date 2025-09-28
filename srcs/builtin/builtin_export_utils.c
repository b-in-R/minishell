/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 18:30:07 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/28 18:34:50 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	export_count(char **env)
{
	int	count;

	count = 0;
	if (!env)
		return (0);
	while (env[count])
		count++;
	return (count);
}

char	**export_copy(char **env)
{
	int		count;
	int		index;
	char	**copy;

	count = export_count(env);
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	index = 0;
	while (index < count)
	{
		copy[index] = env[index];
		index++;
	}
	copy[count] = NULL;
	return (copy);
}

void	export_sort(char **env)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	if (!env)
		return ;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (ft_strcmp(env[i], env[j]) > 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	export_print_value(const char *value)
{
	int	index;

	index = 0;
	while (value[index])
	{
		if (value[index] == '"' || value[index] == '\\')
			ft_putchar_fd('\\', STDOUT_FILENO);
		ft_putchar_fd(value[index], STDOUT_FILENO);
		index++;
	}
}

void	export_print_entry(const char *entry)
{
	int	index;

	index = 0;
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	while (entry[index] && entry[index] != '=')
	{
		ft_putchar_fd(entry[index], STDOUT_FILENO);
		index++;
	}
	if (!entry[index])
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		return ;
	}
	ft_putstr_fd("=\"", STDOUT_FILENO);
	index++;
	export_print_value(entry + index);
	ft_putstr_fd("\"\n", STDOUT_FILENO);
}
