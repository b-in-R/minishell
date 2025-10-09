/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:06:32 by albertooutu       #+#    #+#             */
/*   Updated: 2025/10/09 17:28:14 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* create_cmd:
*	Initialise proprement chaque element de la structure t_cmd.
*	On ne sait pas a l'avance ce que la commande va contenir (args, infile...)
*	C'est pour ça qu'on initialise tous les elements
*	a NULL ou 0 au debut dans create_cmd.
*	Comme ça, le parser va parcourir les tokens
*	et mettre a jour les champs en fonction de ce quil voit.
*	Alloue la memoire dans cmd de taille t_cmd et returne cmd
*/
t_cmd	*create_cmd(t_expander *exp)
{
	t_cmd	*cmd;

	cmd = pool_alloc(exp->pool, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->expand_heredoc = 1;
	cmd->in_fd = -1;
	cmd->delimiter = NULL;
	cmd->double_quote = exp->double_quotes;
	cmd->next = NULL;
	
	printf("create_cmd.c -> create_cmd-> double_quote: %i\n\n", cmd->double_quote);

	return (cmd);
}

/*
* Rajoute une commande () a la fin de la liste chainée t_cmd *cmds(dans parser)
* La 1er fois comme la liste sera vide en rentre dans le if
* et on assigne le new_cmd a *cmd_list
* La 2eme fois on saute la verif car la liste contiendra la 1ere commande
* et tmp pointera vers la 1ere commande, ensuite comme tmp->next est NULL
* on rentre pas dans la boucle et donc tmp->next pointera maintenant sur
* la 2eme commande, donc on aura cmd1 -> cmd2 -> NULL;
*/
void	add_cmd(t_cmd **cmd_list, t_cmd *new_cmd)
{
	t_cmd	*tmp;

	if (!cmd_list || ! new_cmd)
		return ;
	if (*cmd_list == NULL)
	{
		*cmd_list = new_cmd;
		return ;
	}
	tmp = *cmd_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_cmd;
}

/*
* Chaque fois qu'on voit un token de type WORD dans la liste,
* on doit lajouter à current->args (le tableau de mots de la commande actuelle)
*
* - args est un pointeur vers un tableau de chaînes → char **
* - On doit pouvoir :
*     1) créer ce tableau s'il est vide
*     2) ou l'agrandir s'il existe déjà
*
* Comme on doit modifier le champ `args` (le pointeur lui-même) dans la struct
* on a besoin de passer un pointeur vers ce pointeur → char ***
*
* C'est pour cela qu'on appelle : add_arg(&(*current)->args, value);
* ce qui fait que add_arg() reçoit un triple pointeur : char ***
*
* Cela permet à add_arg() de faire : *args = nouveau_tableau
* et donc de mettre à jour le champ `args` directement dans t_cmd.
*
* count → nombre mots dans le tableau;
* +1 → pour le nouveau qu'on rajoute ; +1 pour NULL;
*/
int	add_arg(t_pool *pool, char ***args, const char *value)
{
	char	**new_args;
	char	**old_args;
	int		count;
	int		i;

	count = 0;
	old_args = *args;
	while (old_args && old_args[count])
		count++;
	new_args = pool_alloc(pool, sizeof(char *) * (count + 2));
	if (!new_args)
		return (0);
	i = -1;
	while (++i < count)
		new_args[i] = old_args[i];
	new_args[i] = pool_strdup(pool, value);
	if (!new_args[i])
		return (pool_free_one(pool, new_args), 0);
	new_args[i + 1] = NULL;
	*args = new_args;
	if (old_args)
		pool_free_one(pool, old_args);
	return (1);
}
