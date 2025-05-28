/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:56:16 by albertooutu       #+#    #+#             */
/*   Updated: 2025/05/27 18:58:10 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
* Cree un token on allouant la memorie necessaire pour stocker la structure de chaque token
*et initialise les variables du token value, type et le pointeur vers le prochain token.
*/
t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

/*
* Rajoute un token a la fin de la liste chainée
* La 1er fois comme la liste sera vide en rentre dans le if et on assigne le new_token a *list
* La 2eme fois on saute la verification car la liste contiendra le 1er token
* et tmp pointera vers le 1er token (T1), ensuite comme tmp->next est NULL on rentre pas dans la boucle
* et donc tmp->next pointera maintenant sur le 2eme token (T2), donc on aura T1->T2->NULL
* et comme ça jusqua la fin des tokens
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
