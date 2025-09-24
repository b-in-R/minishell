/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:30:26 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/22 23:15:06 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <errno.h>
#include <limits.h>
#include <string.h>

// Prints arguments separated by spaces, handling the optional -n flag.
static int	has_only_n(char *arg)
{
	int	j;

	j = 1;
	while (arg[j] == 'n')
		j++;
	return (arg[j] == '\0');
}

static int	skip_n_flags(char **args)
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

int	ft_echo(char **args)
{
	int	i;
	int	newline;

	i = skip_n_flags(args);
	newline = (i == 1);
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	return (0);
}

static int	cd_error(const char *msg, const char *detail)
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

static int	cd_errno(const char *path)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (1);
}

static int	cd_set_env(t_expander *exp, const char *key, const char *value)
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

static void	capture_oldpwd(t_expander *exp, char *buf)
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

static int	resolve_target(t_expander *exp, char **args,
		char **target, int *print_new)
{
	*print_new = 0;
	if (!args[1])
	{
		*target = get_env(exp->my_env, "HOME");
		if (!*target)
			return (cd_error("HOME not set", NULL));
		return (0);
	}
	if (!ft_strcmp(args[1], "-"))
	{
		*target = get_env(exp->my_env, "OLDPWD");
		if (!*target)
			return (cd_error("OLDPWD not set", NULL));
		*print_new = 1;
		return (0);
	}
	*target = args[1];
	return (0);
}

static int	finalize_cd(t_expander *exp, char *oldpwd, int print_new)
{
	char	newpwd[PATH_MAX];

	if (!getcwd(newpwd, PATH_MAX))
		return (cd_error("error retrieving current directory", strerror(errno)));
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

// Changes directory, updates PWD/OLDPWD, and mirrors Bash error reporting.
int	ft_cd(t_expander *exp, char **args)
{
	char	oldpwd[PATH_MAX];
	char	*target;
	int		print_new;

	print_new = 0;
	if (args[1] && args[2])
		return (cd_error("too many arguments", NULL));
	capture_oldpwd(exp, oldpwd);
	if (resolve_target(exp, args, &target, &print_new))
		return (1);
	if (chdir(target) == -1)
		return (cd_errno(target));
	return (finalize_cd(exp, oldpwd, print_new));
}

// Outputs the absolute path of the current working directory.
int	ft_pwd(char **my_env, char **args)
{
	char	cwd[1024];
	char	*msg;

	(void)args;
	if (getcwd(cwd, sizeof(cwd)))
	{
		write(STDOUT_FILENO, cwd, ft_strlen(cwd));
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	(void)my_env;
	msg = strerror(errno);
	write(STDERR_FILENO, "minishell: pwd: ", 16);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	return (1);
}
