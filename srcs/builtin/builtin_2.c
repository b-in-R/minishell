/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:50:08 by rabiner           #+#    #+#             */
/*   Updated: 2025/07/14 15:52:26 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// supprime dans local_env et ajoute ou update dans my_env
int	ft_export(t_expander *exp, char **args)
{
	int	i;

	if (!args[1])
		return (print_env(exp->my_env), 0);
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("export: not a valid identifier\n", 2);
			return (1);
		}
		remove_from_env(exp->local_env, args[i]);
		exp->my_env = set_env(exp->my_env, args[i]);
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
