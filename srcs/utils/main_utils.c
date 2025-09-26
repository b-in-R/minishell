/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:09:09 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/26 12:53:58 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	store_only_assignments(t_token *tokens, t_expander *exp)
{
	t_token	*iter;

	iter = tokens;
	while (iter)
	{
		if (add_env_variable(exp->pool, &exp->local_env, iter->value))
		{
			free_tokens(exp->pool, tokens);
			exp->last_status = 1;
			return (-1);
		}
		iter = iter->next;
	}
	free_tokens(exp->pool, tokens);
	return (1);
}

int	handle_assignments(t_token **tokens, t_expander *exp)
{
	int	state;

	if (assignments_only(*tokens))
	{
		state = store_only_assignments(*tokens, exp);
		*tokens = NULL;
		return (state);
	}
	*tokens = discard_assignment_prefix(*tokens, exp->pool);
	if (!*tokens)
		return (1);
	return (0);
}

/*
** Pilote la construction du pipeline: vérifie syntaxe, expansions et heredocs.
*/
void	run_pipeline(t_token *tokens, char *line, t_expander *exp)
{
	t_cmd	*cmds;

	if (check_syntax_errors(tokens, line))
	{
		exp->last_status = 2;
		return ;
	}
	if (!expand_tokens(tokens, exp))
	{
		exp->last_status = 2;
		return ;
	}
	cmds = parser(tokens, exp);
	if (cmds && handle_heredocs(cmds, exp))
		execute(cmds, exp);
	else
	{
		if (!cmds)
			exp->last_status = 1;
		else if (exp->last_status == 0)
			exp->last_status = 1;
	}
	free_cmds(exp->pool, cmds);
}

int	process_tokens(char *line, t_expander *exp)
{
	t_token	*tokens;
	int		state;

	tokens = lexer(line, exp);
	if (!tokens)
		return (1);
	state = handle_assignments(&tokens, exp);
	if (state != 0)
		return (1);
	run_pipeline(tokens, line, exp);
	free_tokens(exp->pool, tokens);
	return (1);
}

int	process_segment(char *line, size_t start, size_t len,
				t_expander *exp)
{
	char	*slice;

	if (len == 0)
		return (1);
	slice = pool_substr_ctx(exp->pool, line, start, len);
	if (!slice)
		return (0);
	process_tokens(slice, exp);
	pool_free_ctx(exp->pool, slice);
	return (1);
}
