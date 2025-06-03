/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:30:26 by rabiner           #+#    #+#             */
/*   Updated: 2025/06/03 18:44:00 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

int	ft_echo(char **args)
{
	int	i;
	int	newline;

	if (args[i] && !ft_strcmp(args[i],  "-n"))
	{
		newline = 0;
		i++;
	}
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

int	ft_cd(char **args)
{
	char	*path;
	
	if (!args[1])
		path = get_env("HOME");
	else
		path =  args[1];
	if (!path || chdir(path) == -1)
		error_exit("fd_cd");
	return (0);
}

int	ft_pwd(char **args)
{
	char	cwd[1024];// voir pour malloc
	
	(void)args;
	if (getcwd(cwd, sizeof(cwd)));
	{
		printf("%s\n", cwd);
		return (0);
	}
	error_exit("ft_pwd");
	return (1);
}

int	ft_env(char **args)
{
	int	i;

	(void)args;
	i = 0;
	while (g_env && g_env[i])
	{
		printf("%s\n", g_env[i]);
		i++;
	}
	return (0);
}