/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:10:25 by albertooutu       #+#    #+#             */
/*   Updated: 2025/07/08 15:25:52 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
t_token	*lexer(char *line)
{
	t_token		*tokens;
	size_t		i;

	tokens = NULL;
	i = 0;
	if (!line || *line == '\0')
		return (NULL);
	while (line[i])
	{
		if (line[i] == ' ')
			i++;
		else if (line[i] == '|')
			handle_pipe(&tokens, &i);
		else if (line[i] == '<' || line[i] == '>')
			handle_redirection(&tokens, line, &i);
		else if (line[i] == '\'' || line[i] == '"' )
			handle_quotes(&tokens, line, &i);
		else
			handle_word(&tokens, line, &i);
	}
	return (tokens);
}
