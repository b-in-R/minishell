/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:11:41 by albertooutu       #+#    #+#             */
/*   Updated: 2025/06/04 16:06:10 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
*	Parser:
*	Reads the list of t_token (tokens) to build structured t_cmd elements
*	(complete commands to execute, every time we find a PIPE (|) we create a t_cmd to execute).
*
*	"Étape"							"Que fait-on ?"
*	- Début							current == NULL, donc on crée une t_cmd vide
*	- Tant qu'on lit un token		On met à jour le champ correspondant de current
*	- Dès qu'on rencontre un PIPE	On remet current = NULL → une nouvelle commande sera créée à la prochaine itération
*	- À chaque fin de boucle		On avance au token suivant (tokens = tokens->next)
*
*/
/*
*	commande:		echo hello > out.txt | grep test < file.txt
*	list tokens:	[WORD: echo] → [WORD: hello] → [REDIR_OUT: >] → [WORD: out.txt] → [PIPE: |] → [WORD: grep] → [WORD: test] → [REDIR_IN: <] → [WORD: file.txt]
*	Résultat final : Deux commandes t_cmd
*					- commande 1: args = {"echo", "hello"} → outfile = "out.txt" → append = 0
*					- commande 2: args = {"grep", "test"} → infile = "file.txt"
*
*/
t_cmd	*parser(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*current;

	cmds = NULL;
	current = NULL;
	while (tokens)
	{
		if (current == NULL)
		{
			current = create_cmd();
			add_cmd(&cmds, current);
		}
		if (tokens->type == WORD)
			add_arg(&current->args, tokens->value);
		else if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT
			|| tokens->type == REDIR_APPEND || tokens->type == HEREDOC)
		{
			handle_redirections(current, &tokens);
		}
		else if (tokens->type == PIPE)
			current = NULL;
		tokens = tokens->next;
	}
	return (cmds);
}

/*
*	Pour racourcir la fonction parser, mets a jour les valeurs des champs correspondants
*/
void	handle_redirections(t_cmd *current, t_token **tokens)
{
	if ((*tokens)->type == REDIR_IN)
	{
		*tokens = (*tokens)->next;
		current->infile = ft_strdup((*tokens)->value);
	}
	else if ((*tokens)->type == REDIR_OUT)
	{
		*tokens = (*tokens)->next;
		current->outfile = ft_strdup((*tokens)->value);
		current->append = 0;
	}
	else if ((*tokens)->type == REDIR_APPEND)
	{
		*tokens = (*tokens)->next;
		current->outfile = ft_strdup((*tokens)->value);
		current->append = 1;
	}
	else if ((*tokens)->type == HEREDOC)
	{
		*tokens = (*tokens)->next;
		current->heredoc = 1;
		current->delimiter = ft_strdup((*tokens)->value);
	}
}
