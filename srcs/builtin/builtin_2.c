/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:50:08 by rabiner           #+#    #+#             */
/*   Updated: 2025/06/25 18:04:38 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_export(char **my_env, char **args)
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
		if (set_env(my_env, args[i]) == -1)
		{
			perror("ft_export");
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_unset(char **my_env, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		unset_env(my_env, args[i]);
		i++;
	}
	return (0);
}
