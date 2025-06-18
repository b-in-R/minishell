/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:51:19 by albertooutu       #+#    #+#             */
/*   Updated: 2025/06/18 16:03:23 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
*	Repérer les erreurs de syntaxe évidentes dans la ligne de commande tokenisée (liste t_token)
*	- Vérifie les pipes en début ou fin de ligne
*	- Vérifie les redirections sans cible (ex: `>`, `<`, `>>`, `<<` sans mot suivant)
*	- Vérifie les pipes consécutifs (ex: `||`)
*	- Vérifie les redirections consécutives (ex: `>> >` ou `<< <`)
*
*	A voir sil faut implementer d'autres vérifications plus complexes comme erreurs de frappe ou ' au debut ou a la fin ou sans fermer
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
		if (tokens->type == PIPE && (!tokens->next || tokens->next->type == PIPE))
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
