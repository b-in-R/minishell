/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:51:19 by albertooutu       #+#    #+#             */
/*   Updated: 2025/09/23 00:17:51 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Returns the type of unclosed quote if any, otherwise 0
 */
static int	is_escaped(const char *line, int index)
{
	int	backslashes;

	backslashes = 0;
	while (index > 0 && line[--index] == '\\')
		backslashes++;
	return (backslashes % 2 != 0);
}

int	has_unclosed_quotes(const char *line)
{
	int	in_single;
	int	in_double;
	int	i;

	in_single = 0;
	in_double = 0;
	i = 0;
	while (line && line[i])
	{
		if (line[i] == '\'' && !in_double && !is_escaped(line, i))
			in_single = !in_single;
		else if (line[i] == '\"' && !in_single && !is_escaped(line, i))
			in_double = !in_double;
		i++;
	}
	return (in_single || in_double);
}

/*
 * Spot obvious syntax errors in the tokenized command line (t_token list)
 * 	- Check for pipes at the beginning or end of a line
 * 	​​- Check for redirections without a target (e.g., `>`, `<`, `>>`, `<<`)
 * 	- Check for consecutive pipes (e.g., `||`)
 * 	- Check for consecutive redirections (e.g., `>> >` or `<< <`)
 */
int	check_syntax_errors(t_token *tokens, char *line)
{
	t_token	*prev;

	(void)line;
	prev = NULL;
	if (!tokens)
		return (1);
	if (has_unclosed_quotes(line))
		return (printf("Syntax error: unclosed quotes\n"), 1);
	if (tokens->type == PIPE)
		return (printf("Syntax error: unexpected pipe\n"), 1);
	while (tokens)
	{
		if (tokens->type == PIPE && (!tokens->next
				|| tokens->next->type == PIPE))
			return (printf("Syntax error: invalid pipe usage\n"), 1);
		if ((tokens->type == REDIR_IN || tokens->type == REDIR_OUT
				|| tokens->type == REDIR_APPEND || tokens->type == HEREDOC)
			&& (!tokens->next || tokens->next->type != WORD))
			return (printf("Syntax error: redirection without target\n"), 1);
		prev = tokens;
		tokens = tokens->next;
	}
	if (prev && prev->type == PIPE)
		return (printf("Syntax error: pipe at end\n"), 1);
	return (0);
}
