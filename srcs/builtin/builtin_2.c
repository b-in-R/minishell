/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:50:08 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/02 17:51:53 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	export_alloc_error(void)
{
	ft_putstr_fd("minishell: export: allocation failed\n", STDERR_FILENO);
	return (1);
}

static int	export_promote_local(t_expander *exp, const char *name, const char *val)
{
	char	*pair;

	pair = ft_strjoin_3(exp->pool, name, "=", val);
	if (!pair)
		return (export_alloc_error());
	if (set_env(exp->pool, &exp->my_env, pair))
	{
		pool_free_ctx(exp->pool, pair);
		return (export_alloc_error());
	}
	remove_from_env(exp->pool, exp->local_env, name);
	pool_free_ctx(exp->pool, pair);
	return (0);
}

static int	export_without_value(t_expander *exp, const char *name)
{
	char	*tmp;

	tmp = pool_strjoin_ctx(exp->pool, name, "=");
	if (!tmp)
		return (export_alloc_error());
	if (set_env(exp->pool, &exp->my_env, tmp))
	{
		pool_free_ctx(exp->pool, tmp);
		return (export_alloc_error());
	}
	pool_free_ctx(exp->pool, tmp);
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
		return (print_env(exp->my_env), 0);
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

// Displays every exported environment entry.
int	ft_env(char **my_env)
{
	print_env(my_env);
	return (0);
}
