/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:30:26 by rabiner           #+#    #+#             */
/*   Updated: 2025/07/14 12:45:58 by albertooutu      ###   ########.fr       */
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
		//write(1, args[i], ft_strlen(args[i]));
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
			//write(1, " ", 1);
		i++;
	}
	if (newline)
		printf("\n");
		//write(1, "\n", 1);
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

	char	*test[] = {"env", NULL};

	(void)args;

	// unset pour test
	ft_unset(my_env, test);
	//

	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		return (0);
	}
	error_exit(my_env, "ft_pwd");
	return (1);
}

int	ft_env(char **my_env)
{
	print_env(my_env);
	return (0);
}
