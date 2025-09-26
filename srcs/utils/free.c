/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 14:16:21 by albertooutu       #+#    #+#             */
/*   Updated: 2025/09/26 12:50:48 by albertooutu      ###   ########.fr       */
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

int	assignments_only(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type != WORD || !is_simple_assignment(tokens->value))
			return (0);
		tokens = tokens->next;
	}
	return (1);
}

t_token	*discard_assignment_prefix(t_token *tokens, t_pool *pool)
{
	t_token	*next;

	while (tokens && tokens->type == WORD
		&& is_simple_assignment(tokens->value))
	{
		next = tokens->next;
		pool_free_ctx(pool, tokens->value);
		pool_free_ctx(pool, tokens);
		tokens = next;
	}
	return (tokens);
}
