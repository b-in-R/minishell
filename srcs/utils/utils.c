/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:09:09 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/10 12:42:05 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
* Creates a copy of the arguments array with outer quotes removed.
 * This is necessary because execve needs the actual command arguments
 * without the shell quotes, but we keep the original args intact
 * for other uses (like echo which needs to preserve inner quotes).
 *
 * Example:
 *		Input:  ["echo", "'hello'", NULL]
 * 		Output: ["echo", "hello", NULL]
 *
 * Returns: A newly allocated array with cleaned arguments.
*/
char	**create_clean_args(char **args)
{
	char	**clean_args;
	int		i;

	i = 0;
	while (args[i])
		i++;
	clean_args = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (args[i])
	{
		clean_args[i] = remove_outer_quotes(args[i]);
		i++;
	}
	clean_args[i] = NULL;
	return (clean_args);
}

void	cleanup_parent(t_cmd *cmd, int *in_fd, int *fd)
{
	if (*in_fd != 0)
	{
		close(*in_fd);
		*in_fd = 0;
	}
	if (cmd->next)
	{
		close(fd[1]);
		*in_fd = fd[0];
	}
	else
	{
		close(fd[0]);
		close(fd[1]);
	}
}

void	error_exit(char **my_env, char *str)
{
	(void)my_env;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
	exit(1);
}

char	*ft_strjoin_3(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, s3);
	free(tmp);
	return (res);
}
