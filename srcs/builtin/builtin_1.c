/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:30:26 by rabiner           #+#    #+#             */
/*   Updated: 2025/10/08 12:35:51 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

// Changes directory, updates PWD/OLDPWD + error handling
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
