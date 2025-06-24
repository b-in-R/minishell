/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:08:26 by albertooutu       #+#    #+#             */
/*   Updated: 2025/06/24 11:36:07 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Aprés avoir été appelé par le lexer:
*  Cree un token de type PIPE avec valeur '|' et l'ajoute à la liste des tokens.
*/
void	handle_pipe(t_token **tokens, size_t *i)
{
	add_token(tokens, create_token(PIPE, ft_strdup("|")));
	(*i)++;
}

/* Aprés avoir été appelé par le lexer:
*  Cree un token de type (valeur): REDIR_IN (<), REDIR_OUT (>), REDIR_APPEND (>>) ou HEREDOC (<<)
*  et l'ajoute à la liste des tokens.
*/
void	handle_redirection(t_token **tokens, char *line, size_t *i)
{
	if (line[*i] == '>' && line [*i + 1] == '>')
	{
		add_token(tokens, create_token(REDIR_APPEND, ft_strdup(">>")));
		*i += 2;
	}
	else if (line[*i] == '<' && line[*i + 1] == '<')
	{
		add_token(tokens, create_token(HEREDOC, ft_strdup("<<")));
		*i += 2;
	}
	else if (line[*i] == '>')
	{
		add_token(tokens, create_token(REDIR_OUT, ft_strdup(">")));
		(*i)++;
	}
	else if (line[*i] == '<')
	{
		add_token(tokens, create_token(REDIR_IN, ft_strdup("<")));
		(*i)++;
	}
}

/* Aprés avoir été appelé par le lexer:
* Lit un mot qui n'est pas entre quotes jusqu'à un espace ou un separateur
*  Crée un token de type WORD avec sa valeur correspondante (word) et l'ajoute a la liste des tokens
*/
void	handle_word(t_token **tokens, char *line, size_t *i)
{
	size_t	start;
	char	*word;

	start = *i;
	while (line[*i] && line[*i] != ' ' && line[*i] != '|' && line[*i] != '\''
		&& line[*i] != '<' && line[*i] != '>' && line[*i] != '"')
	{
		(*i)++;
	}
	word = ft_substr(line, start, *i - start);
	add_token(tokens, create_token(WORD, word));
}

/*
* Lit ce qui est entre quotes ' ou " sans les interpreter
* Extrait la sous chaine a l'interieure et cree un token de type WORD avec valeur word
*/
void	handle_quotes(t_token **tokens, char *line, size_t *i)
{
	char	type_quote;
	char	*word;
	size_t	start;
	t_token	*new_token;

	type_quote = line[*i]; //guillemet ouvrant (le premier)
	start = ++(*i); // caractere suivant au guillemet ouvrant
	while (line[*i] && line[*i] != type_quote)
		(*i)++; // i incrementé jusqu'au dernier guillemet
	word = ft_substr(line, start, *i - start); // extrait le mot/mots placé entre les guillemets
	new_token = create_token(WORD, word);
	if (type_quote == '\'')
		new_token->quoted_type = SINGLE_QUOTE; // si c'est un guillemet simple
	else
		new_token->quoted_type = DOUBLE_QUOTE; // si c'est un guillemet double
	add_token(tokens, new_token);
	if (line[*i] == type_quote)
		(*i)++; // se place dans le caractere suivant au dernier guillemet
}
