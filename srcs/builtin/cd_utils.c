/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 18:26:52 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/30 18:30:36 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	resolve_home_target(t_expander *exp, char **target, const char *suffix)
{
	char	*home;
	char	*joined;

	home = get_env(exp->my_env, "HOME");
	if (!home)
		return (cd_error("HOME not set", NULL));
	if (!suffix || suffix[0] == '\0')
	{
		*target = home;
		return (0);
	}
	joined = pool_strjoin(exp->pool, home, suffix);
	if (!joined)
		return (cd_error("allocation failed", NULL));
	*target = joined;
	return (0);
}

int	resolve_target(t_expander *exp, char **args, char **target, int *print_new)
{
	*print_new = 0;
	if (!args[1])
	{
		return (resolve_home_target(exp, target, NULL));
	}
	if (!ft_strcmp(args[1], "-"))
	{
		*target = get_env(exp->my_env, "OLDPWD");
		if (!*target)
			return (cd_error("OLDPWD not set", NULL));
		*print_new = 1;
		return (0);
	}
	if (args[1][0] == '~' && (!args[1][1] || args[1][1] == '/'))
		return (resolve_home_target(exp, target, args[1] + 1));
	*target = args[1];
	return (0);
}

int	finalize_cd(t_expander *exp, char *oldpwd, int print_new)
{
	char	newpwd[PATH_MAX];

	if (!getcwd(newpwd, PATH_MAX))
		return (cd_error("error retrieving directory", strerror(errno)));
	if (oldpwd[0] && cd_set_env(exp, "OLDPWD", oldpwd))
		return (1);
	if (cd_set_env(exp, "PWD", newpwd))
		return (1);
	if (print_new)
	{
		ft_putstr_fd(newpwd, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	return (0);
}
