/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:08:26 by albertooutu       #+#    #+#             */
/*   Updated: 2025/10/08 13:11:37 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static const char	*resolve_redir(const char *line, size_t i,
				t_token_type *type, size_t *step)
{
	*step = 1;
	if (line[i] == '>' && line[i + 1] == '>')
	{
		*type = REDIR_APPEND;
		*step = 2;
		return (">>");
	}
	if (line[i] == '<' && line[i + 1] == '<')
	{
		*type = HEREDOC;
		*step = 2;
		return ("<<");
	}
	if (line[i] == '>')
	{
		*type = REDIR_OUT;
		return (">");
	}
	if (line[i] == '<')
	{
		*type = REDIR_IN;
		return ("<");
	}
	return (NULL);
}

// After lexer call, create a token PIPE ("|") and add it to the list.
void	handle_pipe(t_lexer *lex)
{
	t_token	*token;

	token = create_token(lex->pool, PIPE, pool_strdup(lex->pool, "|"));
	if (!token)
		return ;
	token->leading_space = *lex->last_space;
	add_token(lex->tokens, token);
	(*lex->index)++;
	*lex->last_space = 0;
}

// After lexer call: create a token redir (<, >, <<, >>) and add it to the list
void	handle_redirection(t_lexer *lex)
{
	t_token_type	type;
	t_token			*token;
	const char		*symbol;
	size_t			step;

	type = WORD;
	symbol = resolve_redir(lex->line, *lex->index, &type, &step);
	if (!symbol)
		return ;
	token = create_token(lex->pool, type, pool_strdup(lex->pool, symbol));
	if (!token)
		return ;
	token->leading_space = *lex->last_space;
	add_token(lex->tokens, token);
	*lex->index += step;
	*lex->last_space = 0;
}

/*
*  After lexer call: read a word tats is not enclosed in quotes until it 
*  encounters a space or separator, create a token WORD with its value,
*  and adds it to the list.
*/
void	handle_word(t_lexer *lex)
{
	size_t	start;
	char	*word;
	t_token	*token;

	start = *lex->index;
	while (lex->line[*lex->index] && ft_isspace(lex->line[*lex->index]) == 0
		&& lex->line[*lex->index] != '|' && lex->line[*lex->index] != '\''
		&& lex->line[*lex->index] != '<' && lex->line[*lex->index] != '>'
		&& lex->line[*lex->index] != '"')
	{
		(*lex->index)++;
	}
	word = pool_substr(lex->pool, lex->line, start, *lex->index - start);
	if (!word)
		return ;
	token = create_token(lex->pool, WORD, word);
	if (!token)
		return ;
	token->leading_space = *lex->last_space;
	add_token(lex->tokens, token);
	*lex->last_space = 0;
}

/*
*	Reads what is between quotes ' or " without interpreting them
*	Extracts the substring inside and create a token WORD with its value.
*/
void	handle_quotes(t_lexer *lex)
{
	char	type_quote;
	char	*word;
	size_t	start;
	t_token	*new_token;

	type_quote = lex->line[*lex->index];
	start = ++(*lex->index);
	while (lex->line[*lex->index] && lex->line[*lex->index] != type_quote)
		(*lex->index)++;
	word = pool_substr(lex->pool, lex->line, start,
			*lex->index - start);
	if (!word)
		return ;
	new_token = create_token(lex->pool, WORD, word);
	if (!new_token)
		return ;
	if (type_quote == '\'')
		new_token->quoted_type = SINGLE_QUOTE;
	else
		new_token->quoted_type = DOUBLE_QUOTE;
	new_token->leading_space = *lex->last_space;
	add_token(lex->tokens, new_token);
	if (lex->line[*lex->index] == type_quote)
		(*lex->index)++;
	*lex->last_space = 0;
}
