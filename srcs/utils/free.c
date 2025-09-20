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

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->value)
			pool_free_ctx(tmp->value);
		if (tmp)
			pool_free_ctx(tmp);
	}
}

void	free_cmds(t_cmd *cmds)
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
				pool_free_ctx(tmp->args[i++]);
			pool_free_ctx(tmp->args);
		}
		if (tmp->infile)
			pool_free_ctx(tmp->infile);
		if (tmp->outfile)
			pool_free_ctx(tmp->outfile);
		if (tmp->delimiter)
			pool_free_ctx(tmp->delimiter);
		pool_free_ctx(tmp);
	}
}

void	free_allocs(char **tofree)
{
	int	i;

	i = 0;
	if (tofree)
	{
		if (tofree[i])
		{
			while (tofree[i])
			{
				pool_free_ctx(tofree[i]);
				i++;
			}
		}
		pool_free_ctx(tofree);
	}
}
