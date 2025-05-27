/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:10:25 by albertooutu       #+#    #+#             */
/*   Updated: 2025/05/27 16:03:38 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
/*	Lexer:
*	Converts the raw input line (char *line) into a linked list of t_token elements.
*/

/*
*	1. Scanner la ligne caractère par caractère
*	2. Selon le caractère actuel :
*		- ignorer les espaces (ls -la)
*		- capturer un pipe (|)
*		- détecter les redirections (>, <, >>, <<)
*		- lire un mot simple
*		- capturer ce qui est entre quotes simples ' ou doubles "
*	3. À chaque "morceau identifié", créer un nouveau token avec le bon type et la valeur copiée.
*	4. Ajouter chaque token à une liste chaînée.
*/

t_token	*lexer(char *line)
{
	return (NULL);
}
