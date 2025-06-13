/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:10:50 by albertooutu       #+#    #+#             */
/*   Updated: 2025/06/13 15:27:24 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
*	Expander:
*	Replaces environment variables like $VAR and $? inside tokens of type WORD.
*/
void	expand_tokens(t_token *tokens, int last_status)
{
	char	*new_value;

	while (tokens)
	{
		if (tokens->type == WORD)
		{
			new_value = expand_word(tokens->value, last_status);
			free(tokens->value); // Libère l'ancienne valeur
			tokens->value = new_value; // Assigne la nouvelle valeur
		}
		tokens = tokens->next;
	}
}

/*
*	traiter plusieurs expansions dans la meme chaine (plusieurs $VAR)
*	prendre en compte les quotes:  '' no expansion et "" expansion
*	mettre a jour last_status si on trouve un $?
*	mettre a jour le token avec la nouvelle valeur si on trouve $VAR
*/
char	*expand_word(const char *word, int last_status)
{
	int		i;
	int		in_single;
	int		in_double;
	char	*result; // Chaîne de résultat qui va contenir le mot après expansion

	i = 0;
	in_single = 0;
	in_double = 0;
	result = ft_strdup(""); // Commence avec une chaîne vide
	while (word[i])
	{
		update_quote_flags(word[i], &in_single, &in_double); // Met à jour les flags de quotes
		if (word[i] == '$' && !in_single)
			handle_dollar(word, &i, &result, last_status); // Gère l'expansion des variables
		else
			append_char(&result, word[i++]); // Ajoute le caractère courant à result
	}
	return (result); // Retourne la chaîne résultante après expansion
}
