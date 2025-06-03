/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:50:08 by rabiner           #+#    #+#             */
/*   Updated: 2025/06/03 18:57:22 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

int	ft_export(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (!ft_strchr(args[i], '='))
		{
			printf("export: invalid format: %s\n", args[i]);
			i++;
			continue ;
		}
		if (set_env(args[i]) == -1)
		{
			perror("ft_export");
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_unset(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		unset_env(args[i]);
		i++;
	}
	return (0);
}
