/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:08:26 by albertooutu       #+#    #+#             */
/*   Updated: 2025/09/23 00:09:19 by rabiner          ###   ########.fr       */
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
void	handle_pipe(t_lexer_ctx *ctx)
{
	t_token	*token;

	token = create_token(ctx->pool, PIPE, pool_strdup_ctx(ctx->pool, "|"));
	if (!token)
		return ;
	token->leading_space = *ctx->last_space;
	add_token(ctx->tokens, token);
	(*ctx->index)++;
	*ctx->last_space = 0;
}

/*
** Après appel du lexer: crée un token de redirection ( <, >, >>, << )
** et l'ajoute à la liste des tokens.
*/
void	handle_redirection(t_lexer_ctx *ctx)
{
	t_token_type	type;
	t_token		*token;
	const char	*symbol;
	size_t		step;

	type = WORD;
	symbol = resolve_redir(ctx->line, *ctx->index, &type, &step);
	if (!symbol)
		return ;
	token = create_token(ctx->pool, type, pool_strdup_ctx(ctx->pool, symbol));
	if (!token)
		return ;
	token->leading_space = *ctx->last_space;
	add_token(ctx->tokens, token);
	*ctx->index += step;
	*ctx->last_space = 0;
}

/* Aprés avoir été appelé par le lexer:
* Lit un mot qui n'est pas entre quotes jusqu'à un espace ou un separateur
*  Crée un token de type WORD avec sa valeur correspondante (word)
* et l'ajoute a la liste des tokens
*/
void	handle_word(t_lexer_ctx *ctx)
{
	size_t	start;
	char	*word;
	t_token	*token;

	start = *ctx->index;
	while (ctx->line[*ctx->index] && ft_isspace(ctx->line[*ctx->index]) == 0
		&& ctx->line[*ctx->index] != '|' && ctx->line[*ctx->index] != '\''
		&& ctx->line[*ctx->index] != '<' && ctx->line[*ctx->index] != '>'
		&& ctx->line[*ctx->index] != '"')
	{
		(*ctx->index)++;
	}
	word = pool_substr_ctx(ctx->pool, ctx->line, start, *ctx->index - start);
	if (!word)
		return ;
	token = create_token(ctx->pool, WORD, word);
	if (!token)
		return ;
	token->leading_space = *ctx->last_space;
	add_token(ctx->tokens, token);
	*ctx->last_space = 0;
}

/*
* Lit ce qui est entre quotes ' ou " sans les interpreter
* Extrait la sous chaine a l'interieure
* et cree un token de type WORD avec valeur word
*/

// rabiner: ' " ne fais pas le retour a la ligne avec > 

void	handle_quotes(t_lexer_ctx *ctx)
{
	char	type_quote;
	char	*word;
	size_t	start;
	t_token	*new_token;

	type_quote = ctx->line[*ctx->index];
	start = ++(*ctx->index);
	while (ctx->line[*ctx->index] && ctx->line[*ctx->index] != type_quote)
		(*ctx->index)++;
	word = pool_substr_ctx(ctx->pool, ctx->line, start,
			*ctx->index - start);
	if (!word)
		return ;
	new_token = create_token(ctx->pool, WORD, word);
	if (!new_token)
		return ;
	if (type_quote == '\'')
		new_token->quoted_type = SINGLE_QUOTE;
	else
		new_token->quoted_type = DOUBLE_QUOTE;
	new_token->leading_space = *ctx->last_space;
	add_token(ctx->tokens, new_token);
	if (ctx->line[*ctx->index] == type_quote)
		(*ctx->index)++;
	*ctx->last_space = 0;
}
