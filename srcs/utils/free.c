/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 14:16:21 by albertooutu       #+#    #+#             */
/*   Updated: 2025/09/20 00:07:27 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_tokens(t_pool *pool, t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->value)
			pool_free_ctx(pool, tmp->value);
		pool_free_ctx(pool, tmp);
	}
}

void	free_cmds(t_pool *pool, t_cmd *cmds)
{
	t_cmd	*tmp;
	int		i;

	while (cmds)
	{
		tmp = cmds;
		cmds = cmds->next;
		if (tmp->args)
		{
			i = 0;
			while (tmp->args[i])
				pool_free_ctx(pool, tmp->args[i++]);
			pool_free_ctx(pool, tmp->args);
		}
		if (tmp->infile)
			pool_free_ctx(pool, tmp->infile);
		if (tmp->outfile)
			pool_free_ctx(pool, tmp->outfile);
		if (tmp->delimiter)
			pool_free_ctx(pool, tmp->delimiter);
		pool_free_ctx(pool, tmp);
	}
}

void	free_allocs(t_pool *pool, char **tofree)
{
	int	i;

	i = 0;
	if (tofree)
	{
		if (tofree[i])
		{
			while (tofree[i])
			{
				pool_free_ctx(pool, tofree[i]);
				i++;
			}
		}
		pool_free_ctx(pool, tofree);
	}
}
