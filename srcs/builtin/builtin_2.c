/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:50:08 by rabiner           #+#    #+#             */
/*   Updated: 2025/08/18 15:31:19 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// pour le cas "export NAME" (sans '=').
static int	export_name_only(t_expander *exp, const char *name)
{
	char	*val;
	char	*pair;
	char	*tmp;

	val = get_env(exp->local_env, (char *)name);
	if (val)
	{
		pair = ft_strjoin_3((char *)name, "=", val);
		if (!pair)
			return (1);
		exp->my_env = set_env(exp->my_env, pair);
		remove_from_env(exp->local_env, name);
		free(pair);
		return (0);
	}
	if (get_env(exp->my_env, (char *) name))
		return (0);
	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (1);
	exp->my_env = set_env(exp->my_env, tmp);
	free(tmp);
	return (0);
}

// supprime dans local_env et ajoute ou update dans my_env
int	ft_export(t_expander *exp, char **args)
{
	int		i;
	int		ret;

	i = 1;
	ret = 0;
	if (!args[1])
		return (print_env(exp->my_env), 0);
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ret = (ft_putstr_fd("export: not a valid identifier\n", 2), 1);
			i++;
			continue ;
		}
		if (!ft_strchr(args[i], '='))
			ret |= export_name_only(exp, args[i]);
		else
		{
			remove_from_env(exp->local_env, args[i]);
			exp->my_env = set_env(exp->my_env, args[i]);
		}
		i++;
	}
	return (ret);
}

/*
int	ft_unset(char **my_env, char **args)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("unset: not a valid identifier\n", 2);
			ret = 1;
		}
		else
			remove_from_env(my_env, args[i]);
		i++;
	}
	return (ret);
}
*/

int	ft_unset(char **my_env, char **args)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			ret = 1;
		}
		else
			remove_from_env(my_env, args[i]);
		i++;
	}
	return (ret);
}

int	ft_env(char **my_env)
{
	print_env(my_env);
	return (0);
}
