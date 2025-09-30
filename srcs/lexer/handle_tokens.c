/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:08:26 by albertooutu       #+#    #+#             */
/*   Updated: 2025/09/25 17:24:28 by albertooutu      ###   ########.fr       */
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

/*
** Après appel du lexer: crée un token PIPE ("|") et l'ajoute à la liste.
*/
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

/*
** Après appel du lexer: crée un token de redirection ( <, >, >>, << )
** et l'ajoute à la liste des tokens.
*/
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

/* Aprés avoir été appelé par le lexer:
* Lit un mot qui n'est pas entre quotes jusqu'à un espace ou un separateur
*  Crée un token de type WORD avec sa valeur correspondante (word)
* et l'ajoute a la liste des tokens
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
* Lit ce qui est entre quotes ' ou " sans les interpreter
* Extrait la sous chaine a l'interieure
* et cree un token de type WORD avec valeur word
*/

// rabiner: ' " ne fais pas le retour a la ligne avec >

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
