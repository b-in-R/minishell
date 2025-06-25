/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:30:26 by rabiner           #+#    #+#             */
/*   Updated: 2025/06/25 18:37:09 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (args[i] && !ft_strcmp(args[i], "-n"))
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

int	ft_cd(char **my_env, char **args)
{
	char	*path;
	
	if (!args[1])
		path = get_env(my_env, "HOME");
	else
		path =  args[1];
	if (!path || chdir(path) == -1)
		error_exit(my_env, "fd_cd");
	return (0);
}

int	ft_pwd(char **my_env, char **args)
{
	char	cwd[1024];// voir pour malloc
	
	(void)args;
	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		return (0);
	}
	error_exit(my_env, "ft_pwd");
	return (1);
}

int	ft_env(char **my_env, char **args)// suppr args
{
	int	i;

	(void)args;
	i = 0;
	// test
	//printf(YELL"ft_env -> start ok\n"RST);
	//printf(YELL"g_env[%i]:\t%s\n"RST, i, g_env[i]);

	while (my_env && my_env[i])
	{
		//test
		//if (i == 0)
			//printf(YELL"ft_env -> while ok\n"RST);
		//printf(YELL"g_env[%i]:\t%s\n"RST, i, my_env[i]);
			
		printf("%s\n", my_env[i]);
		i++;
	}
	return (0);
}