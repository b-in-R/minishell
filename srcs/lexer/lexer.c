/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:10:25 by albertooutu       #+#    #+#             */
/*   Updated: 2025/09/28 22:42:50 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	lexer_loop(t_lexer *lex)
{
	char	*line;
	size_t	*i;
	int		*last_space;

	line = lex->line;
	i = lex->index;
	last_space = lex->last_space;
	while (line[*i])
	{
		if (line[*i] == ' ' || (line[*i] >= '\t' && line[*i] <= '\r'))
		{
			while (line[*i] == ' ' || (line[*i] >= '\t' && line[*i] <= '\r'))
				(*i)++;
			*last_space = 1;
		}
		else if (line[*i] == '|')
			handle_pipe(lex);
		else if (line[*i] == '<' || line[*i] == '>')
			handle_redirection(lex);
		else if (line[*i] == '\'' || line[*i] == '"')
			handle_quotes(lex);
		else
			handle_word(lex);
	}
}

/* Lexer:
*Converts the raw input line (char *line) into a linked list of t_token elemnts
* Prends une ligne de commande brute (ex: echo "hello" > out.txt)
* La scanne caractere par caractere et la découpe en tokens, ignore les spaces
* Associe à chaque token son type (WORD, PIPE, REDIR_OUT, etc.)
* Crée une liste chaînée de t_token
* Retourner cette liste chaînée pour qu’elle soit utilisée ensuite par le parse
*/
/* Résumé du flux de traitement:
*readline() --> line : "echo 'hello > test' | grep ok >> output"
*lexer()
* ├─ "echo"     → WORD
* ├─ "'hello > test'" → WORD (quotes simples → tout pris littéral)
* ├─ "|"        → PIPE
* ├─ "grep"     → WORD
* ├─ "ok"       → WORD
* ├─ ">>"       → REDIR_APPEND
* └─ "output"   → WORD
*/
t_token	*lexer(char *line, t_expander *exp)
{
	t_token			*tokens;
	static size_t	i;
	static int		last_space;
	t_lexer			lex;

	tokens = NULL;
	if (!line || *line == '\0')
		return (NULL);
	i = 0;
	last_space = 1;
	lex.tokens = &tokens;
	lex.line = line;
	lex.index = &i;
	lex.last_space = &last_space;
	lex.pool = exp->pool;
	lexer_loop(&lex);
	return (tokens);
}
