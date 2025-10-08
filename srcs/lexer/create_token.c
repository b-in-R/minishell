/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:56:16 by albertooutu       #+#    #+#             */
/*   Updated: 2025/10/08 13:21:43 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
* Create a token with the right memory space to hold the structs of all tokens
* and initialise variables.
*/
t_token	*create_token(t_pool *pool, t_token_type type, char *value)
{
	t_token	*token;

	token = pool_alloc(pool, sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->quoted_type = NO_QUOTE;
	token->leading_space = 1;
	token->pool = pool;
	token->next = NULL;
	return (token);
}


/*
* Add a token to the end of the linked list
*/
void	add_token(t_token **list, t_token *new_token)
{
	t_token	*tmp;

	if (!*list)
		*list = new_token;
	else
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_token;
	}
}
