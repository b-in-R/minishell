/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:51:19 by albertooutu       #+#    #+#             */
/*   Updated: 2025/07/08 16:02:09 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
* Spot obvious syntax errors in the tokenized command line (t_token list)
* 	- Check for pipes at the beginning or end of a line
* 	​​- Check for redirections without a target (e.g., `>`, `<`, `>>`, `<<`)
* 	- Check for consecutive pipes (e.g., `||`)
* 	- Check for consecutive redirections (e.g., `>> >` or `<< <`)
*/
int	check_syntax_errors(t_token *tokens)
{
	t_token	*prev;

	prev = NULL;
	if (!tokens)
		return (1);
	if (tokens->type == PIPE)
		return (printf("Syntax error: unexpected pipe\n"), 1);
	while (tokens)
	{
		if (tokens->type == PIPE
			&& (!tokens->next || tokens->next->type == PIPE))
			return (printf("Syntax error: invalid pipe usage\n"), 1);
		if ((tokens->type == REDIR_IN || tokens->type == REDIR_OUT
				|| tokens->type == REDIR_APPEND || tokens->type == HEREDOC)
			&& (!tokens->next || tokens->next->type != WORD))
		{
			return (printf("Syntax error: redirection without target\n"), 1);
		}
		prev = tokens;
		tokens = tokens->next;
	}
	if (prev && prev->type == PIPE)
		return (printf("Syntax error: pipe at end\n"), 1);
	return (0);
}
