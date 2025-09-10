/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:30:26 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/10 12:23:28 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_echo(char **args)
{
	int	i;
	int	j;
	int	newline;

	i = 1;
	newline = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 1;
		while (args[i][j++] == 'n')
		if (args[i][j - 1] != '\0')
			break ;
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
		path = args[1];
	if (!path || chdir(path) == -1)
	{
		write(2, "minishell: cd: n'est pas un dossier: ", 37);
		write(2, path, ft_strlen(path));
		write(2, "\n", 1);
		return (1);
	}
	return (0);
}

int	ft_pwd(char **my_env, char **args)
{
	char cwd[1024];

	(void)args;
	if (getcwd(cwd, sizeof(cwd)))
	{
		write(STDOUT_FILENO, cwd, ft_strlen(cwd));
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	error_exit(my_env, "ft_pwd");
	return (1);
}
