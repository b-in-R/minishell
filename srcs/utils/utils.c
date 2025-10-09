/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:09:09 by rabiner           #+#    #+#             */
/*   Updated: 2025/10/09 16:44:57 by rabiner          ###   ########.fr       */
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
char	**create_clean_args(t_pool *pool, char **args, t_cmd *cmd)
{
	char	**clean_args;
	int		i;

	if (!args || !args[0])
	{
		clean_args = pool_alloc(pool, sizeof(char *));
		if (!clean_args)
			return (NULL);
		clean_args[0] = NULL;
		return (clean_args);
	}
	i = 0;
	while (args[i])
		i++;
	clean_args = pool_alloc(pool, sizeof(char *) * (i + 1));
	if (!clean_args)
		return (NULL);
	clean_args[i] = NULL;
	while (i--)
	{
		clean_args[i] = remove_outer_quotes(pool, args[i], cmd);
		if (!clean_args[i])
			return (NULL);
	}
	return (clean_args);
}

void	cleanup_parent(t_pool *pool, t_cmd *cmd, int *in_fd, int *fd)
{
	if (*in_fd != 0 && *in_fd != -1)
	{
		pool_close_one(pool, *in_fd);
		*in_fd = 0;
	}
	if (cmd->next && fd[1] > 0)
	{
		pool_close_one(pool, fd[1]);
		*in_fd = fd[0];
	}
	else if (fd[0] > 0 && fd[1] > 0)
	{
		pool_close_one(pool, fd[0]);
		pool_close_one(pool, fd[1]);
	}
}

void	error_exit(t_pool *pool, char **my_env, const char *str)
{
	(void)my_env;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd((char *)str, 2);
	ft_putstr_fd("\n", 2);
	if (pool)
		pool_cleanup(pool);
	exit(1);
}

char	*ft_strjoin_3(t_pool *pool, const char *s1, const char *s2,
		const char *s3)
{
	char	*tmp;
	char	*res;

	tmp = pool_strjoin(pool, s1, s2);
	if (!tmp)
		return (NULL);
	res = pool_strjoin(pool, tmp, s3);
	if (!res)
	{
		pool_free_one(pool, tmp);
		return (NULL);
	}
	pool_free_one(pool, tmp);
	return (res);
}

//TESTS
void	print_detailled_cmds(t_cmd *cmds)
{
	int	i = 0;

	printf("\n\n### Detailled ###\n");
	while (cmds->args[i])
	{
		printf("args[%i]:\t\t%s\n", i, cmds->args[i]);
		i++;
	}
	printf("infile:\t\t\t%s\n", cmds->infile);
	printf("outfile:\t\t%s\n", cmds->outfile);
	printf("append:\t\t\t%i\n", cmds->append);
	printf("heredoc:\t\t%i\n", cmds->heredoc);
	printf("expand_heredoc:\t\t%i\n", cmds->expand_heredoc);
	printf("in_fd:\t\t\t%i\n", cmds->in_fd);
	printf("delimiter:\t\t%s\n", cmds->delimiter);
	printf("double_quote:\t\t%i\n", cmds->double_quote);
}


//TEMPORAIRE : fonction de test pour montrer les cmds
void print_cmds(t_cmd *cmds)
{
	int	i = 1;
	while (cmds)
	{
		printf("----------- CMD  %d -----------\n", i++);
		if (cmds->args)
		{
			printf("Args: ");
			print_detailled_cmds(cmds);
			printf("\n");
		}
		cmds = cmds->next;
	}
	printf("------------------------------\n\n");
}
