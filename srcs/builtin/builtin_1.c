/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:30:26 by rabiner           #+#    #+#             */
/*   Updated: 2025/08/18 15:02:28 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_echo(char **args)
{
	int	i;
	int	j;

	i = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ;
		i++;
	}
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!(args[1][0] == '-' && args[1][1] == 'n'))
		write(1, "\n", 1);
	return (0);
}

int	ft_cd(char **my_env, char **args)
{
	char	*path;

	if (!args[1])
		path = get_env(my_env, "HOME");
	else
		path = args[1];
	if (!path || chdir(path) == -1)
	{
		write(1, "minishell: cd: n'est pas un dossier: ", 37);
		write(1, path, ft_strlen(path));
		write(1, "\n", 1);
	}
	return (0);
}

// voir pour malloc cwd
int	ft_pwd(char **my_env, char **args)
{
	char	cwd[1024];

	(void)args;
	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		return (0);
	}
	error_exit(my_env, "ft_pwd");
	return (1);
}
