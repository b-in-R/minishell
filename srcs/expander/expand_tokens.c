/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:10:50 by albertooutu       #+#    #+#             */
/*   Updated: 2025/06/24 15:18:35 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
int	handle_dollar(const char *word, int *i, char **result, int last_status);
int	append_char(char **str, char c);

/*
*	sert à suivre si on est à l’intérieur de quotes simples (') ou doubles ("), pendant qu’on parcourt une chaîne de caractères (un token WORD).
*	Si on rencontre un simple quote ('), (et qu’on n’est pas dans des "")  on inverse in_single.
* 	Si on rencontre un double quote ("), (et qu’on n’est pas dans des '') on inverse in_double.
*/
void	update_quote_flags(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !(*in_double))
		*in_single = !*in_single;
	else if (c == '"' && !(*in_single))
		*in_double = !*in_double;
}

/*
*	Expander:
*	Replaces environment variables like $VAR and $? inside tokens of type WORD.
*/
int	expand_tokens(t_token *tokens, int last_status)
{
	char	*new_value;

	while (tokens)
	{
		if (tokens->type == WORD && tokens->quoted_type != SINGLE_QUOTE)
		{
			new_value = expand_word(tokens->value, last_status);
			if (!new_value)
				return (0);
			free(tokens->value); // Libère l'ancienne valeur
			tokens->value = new_value; // Assigne la nouvelle valeur
		}
		tokens = tokens->next;
	}
	return (1); // Retourne 1 si l'expansion a réussi
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
	if (!result)
		return (NULL);
	while (word[i])
	{
		update_quote_flags(word[i], &in_single, &in_double); // Met à jour les flags de quotes
		if (word[i] == '$')
		{
			if (!handle_dollar(word, &i, &result, last_status))
				return (free(result), NULL);
		}
		else
		{
			if (!append_char(&result, word[i++]))
				return (free(result), NULL);
		}
	}
	return (result); // Retourne la chaîne résultante après expansion
}
