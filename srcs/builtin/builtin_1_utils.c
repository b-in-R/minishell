/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:30:26 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/26 12:47:33 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <errno.h>
#include <limits.h>
#include <string.h>

// Prints arguments separated by spaces, handling the optional -n flag.
int	has_only_n(char *arg)
{
	int	j;

	j = 1;
	while (arg[j] == 'n')
		j++;
	return (arg[j] == '\0');
}

int	skip_n_flags(char **args)
{
	int	i;

	i = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		if (!has_only_n(args[i]))
			break ;
		i++;
	}
	return (i);
}

int	cd_error(const char *msg, const char *detail)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	if (detail)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(detail, STDERR_FILENO);
	}
	ft_putstr_fd("\n", STDERR_FILENO);
	return (1);
}

int	cd_errno(const char *path)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (1);
}

int	cd_set_env(t_expander *exp, const char *key, const char *value)
{
	char	*pair;

	pair = ft_strjoin_3(exp->pool, key, "=", value);
	if (!pair)
		return (cd_error("allocation failed", NULL));
	if (set_env(exp->pool, &exp->my_env, pair))
	{
		pool_free_ctx(exp->pool, pair);
		return (cd_error("allocation failed", NULL));
	}
	remove_from_env(exp->pool, exp->local_env, key);
	pool_free_ctx(exp->pool, pair);
	return (0);
}
