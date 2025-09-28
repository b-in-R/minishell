/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:50:08 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/28 18:32:11 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	export_print(char **env)
{
	int		index;
	char	**sorted;

	sorted = export_copy(env);
	if (!sorted)
		return (export_alloc_error());
	export_sort(sorted);
	index = 0;
	while (sorted[index])
	{
		export_print_entry(sorted[index]);
		index++;
	}
	free(sorted);
	return (0);
}

// Promotes NAME from local env to export when no value is provided.
static int	export_name_only(t_expander *exp, const char *name)
{
	char	*val;

	val = get_env(exp->local_env, (char *)name);
	if (val)
		return (export_promote_local(exp, name, val));
	if (get_env(exp->my_env, (char *)name))
		return (0);
	return (export_without_value(exp, name));
}

static int	export_handle_arg(t_expander *exp, const char *arg)
{
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: '", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return (1);
	}
	if (!ft_strchr(arg, '='))
		return (export_name_only(exp, arg));
	if (set_env(exp->pool, &exp->my_env, arg))
	{
		export_alloc_error();
		return (1);
	}
	remove_from_env(exp->pool, exp->local_env, arg);
	return (0);
}

// supprime dans local_env et ajoute ou update dans my_env
// Adds or updates environment variables in the exported scope.
int	ft_export(t_expander *exp, char **args)
{
	int		i;
	int		ret;

	i = 1;
	ret = 0;
	if (!args[1])
		return (export_print(exp->my_env));
	while (args[i])
	{
		if (export_handle_arg(exp, args[i]))
			ret = 1;
		i++;
	}
	return (ret);
}

// Removes variables from both exported and local environments.
int	ft_unset(t_expander *exp, char **args)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: unset: '", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			ret = 1;
		}
		else
		{
			remove_from_env(exp->pool, exp->my_env, args[i]);
			remove_from_env(exp->pool, exp->local_env, args[i]);
		}
		i++;
	}
	return (ret);
}
