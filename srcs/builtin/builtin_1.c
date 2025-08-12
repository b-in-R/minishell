/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:30:26 by rabiner           #+#    #+#             */
/*   Updated: 2025/08/12 15:55:42 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_echo(char **args)
{
	int	i;
	int	j;
//	int	n;

	i = 1;
//	n = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break;
//		n = 1;
		i++;
	}
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
//	if (n)
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
