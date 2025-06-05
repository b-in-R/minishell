/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:10:50 by albertooutu       #+#    #+#             */
/*   Updated: 2025/06/05 11:38:01 by albertooutu      ###   ########.fr       */
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

	i = 0;
	while (word[i])
	{
	}
}

// obtenir la valuer de l'environnement ($USER, $PATH) avec getenv()
char	*get_env_value(const char *word)
{
	char	*value;

	value = getenv(word);
	if (value)
		return (ft_strdup(value)); // Duplique la valeur trouvée
	else
		return (ft_strdup("")); // Si pas trouvé, retourne une chaîne vide
}
