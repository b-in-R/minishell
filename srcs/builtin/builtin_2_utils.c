/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:30:26 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/26 12:47:51 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <errno.h>
#include <limits.h>
#include <string.h>

void	capture_oldpwd(t_expander *exp, char *buf)
{
	char	*env_pwd;

	if (getcwd(buf, PATH_MAX))
		return ;
	env_pwd = get_env(exp->my_env, "PWD");
	if (env_pwd)
		ft_strlcpy(buf, env_pwd, PATH_MAX);
	else
		buf[0] = '\0';
}

int	export_alloc_error(void)
{
	ft_putstr_fd("minishell: export: allocation failed\n", STDERR_FILENO);
	return (1);
}

int	export_promote_local(t_expander *exp, const char *name,
	const char *val)
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

int	export_without_value(t_expander *exp, const char *name)
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
