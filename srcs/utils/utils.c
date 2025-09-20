/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:09:09 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/20 00:08:11 by rabiner          ###   ########.fr       */
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
	if (*in_fd != 0 && *in_fd != -1)
	{
		pool_close_ctx(*in_fd);
		*in_fd = 0;
	}
	if (cmd->next && fd[1] > 0)
	{
		pool_close_ctx(fd[1]);
		*in_fd = fd[0];
	}
	else if (fd[0] > 0 && fd[1] > 0)
	{
		pool_close_ctx(fd[0]);
		pool_close_ctx(fd[1]);
	}
}

void	error_exit(char **my_env, char *str)
{
	(void)my_env;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
	pool_cleanup_ctx();
	exit(1);
}

char	*ft_strjoin_3(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*res;

	tmp = pool_strjoin_ctx(s1, s2);
	if (!tmp)
		return (NULL);
	res = pool_strjoin_ctx(tmp, s3);
	if (!res)
	{
		pool_free_ctx(tmp);
		return (NULL);
	}
	pool_free_ctx(tmp);
	return (res);
}
/*
//TEMPORAIRE
void	print_detailled_cmds(t_cmd *cmds)
{
	int	i = 0;

	printf("\n\n### Detailled ###\n");
	while (cmds->args[i])
	{
		printf("args[%i]:\t%s\n", i, cmds->args[i]);
		i++;
	}
	printf("infile:\t\t%s\n", cmds->infile);
	printf("outfile:\t%s\n", cmds->outfile);
	printf("append:\t\t%i\n", cmds->append);
	printf("heredoc:\t%i\n", cmds->heredoc);
	printf("delimiter:\t%s\n", cmds->delimiter);
}


//TEMPORAIRE : fonction de test pour montrer les cmds
void print_cmds(t_cmd *cmds)
{
	int	i = 1;
	while (cmds)
	{
		printf("-- CMD %d --\n", i++);
		if (cmds->args)
		{
			printf("Args: ");
			for (int j = 0; cmds->args[j]; j++)
			{
				//printf("\n[%s] ", cmds->args[j]);
				print_detailled_cmds(cmds);
			}
			printf("\n");
		}
		if (cmds->infile)
			printf("Infile: %s\n", cmds->infile);
		if (cmds->outfile)
			printf("Outfile: %s (%s)\n", cmds->outfile, \
			cmds->append ? "append" : "truncate");
		if (cmds->heredoc)
			printf("Heredoc delimiter: %s\n", cmds->delimiter);
		cmds = cmds->next;
	}
	printf("-----------------------\n\n");
}
	*/
