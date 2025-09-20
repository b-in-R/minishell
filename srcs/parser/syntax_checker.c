/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:51:19 by albertooutu       #+#    #+#             */
/*   Updated: 2025/09/08 16:17:17 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 *	Detecte sil y a une quote simple ou double non fermee
 *	Parcours la ligne caractere par caractere
 *	Quand une quote est ouverte on la stocke
 *	Si on retombe sur le meme type de quote apres,
 *		ca veut dire quelle est fermee donc a la fin on retourne 0
 *	A la fin, si une quote est ouverte en retoune erreur
 */

 /*
int	has_unclosed_quotes(const char *line)
{
	char	quote;
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')// ' ou "	'
		{
			quote = line[i];
			i++;
			while (line[i] && line[i] != quote)
				i++;
			if (line[i] != quote)
				return (1);
		}
		else
			i++;
	}
	return (0);
}
*/

// modif rabiner
int	has_unclosed_quotes(const char *line)
{
	int		i;
	int		single_quote;
	int		double_quote;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (line[i])
	{
		if (line[i] == '\'')
			single_quote++;
		else if (line[i] == '\"')
			double_quote++;
		i++;
	}
	if ((single_quote % 2 != 0) || (double_quote % 2 != 0))
		return (1);
	return (0);
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
