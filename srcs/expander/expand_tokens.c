/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:10:50 by albertooutu       #+#    #+#             */
/*   Updated: 2025/07/14 15:53:33 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_dollar(const char *word, int *i, char **result, t_expander *exp);
int	append_char(char **str, char c);

/*
* Used to track whether you're inside single quotes (')
*	or double quotes ("), while traversing a string (a WORD token).
* If encounter a single quote ('), (and not inside ""), reverse in_single.
* If encounter a double quote ("), (and not inside "'), reverse in_double.
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
*	Replaces environment variables like $VAR and $? inside tokens of type WORD
*	Iterates through the linked list of tokens and expands each WORD token
*	Free old value of the token and assign the new expanded value.
*/
int	expand_tokens(t_token *tokens, t_expander *exp)
{
	char	*new_value;

	while (tokens)
	{
		if (tokens->type == WORD && tokens->quoted_type != SINGLE_QUOTE)
		{
			new_value = expand_word(tokens->value, exp);
			if (!new_value)
				return (0);
			free(tokens->value);
			tokens->value = new_value;
		}
		tokens = tokens->next;
	}
	return (1);
}

/*
* Process multiple expansions in the same string (multiple $VARs)
* Take into account quotes: '' no expansion and "" expansion
* Update last_status if a $? is found
* Update the token with the new value if $VAR is found
*/
char	*expand_word(const char *word, t_expander *exp)
{
	int		i;
	int		in_single;
	int		in_double;
	char	*result;

	i = 0;
	in_single = 0;
	in_double = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (word[i])
	{
		update_quote_flags(word[i], &in_single, &in_double);
		if (word[i] == '$')
		{
			if (!handle_dollar(word, &i, &result, exp))
				return (free(result), NULL);
		}
		else if (!append_char(&result, word[i++]))
			return (free(result), NULL);
	}
	return (result);
}

/*
*	Joins all tokens into a single string, separating them with spaces
*/
char	*join_tokens(t_token *tokens)
{
	char	*result;
	t_token	*curr;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	curr = tokens;
	while (curr)
	{
		if (!str_append_free(&result, curr->value))
			return (free(result), NULL);
		if (curr->next)
		{
			if (!str_append_free(&result, " "))
				return (free(result), NULL);
		}
		curr = curr->next;
	}
	return (result);
}
