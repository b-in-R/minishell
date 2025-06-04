/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:06:32 by albertooutu       #+#    #+#             */
/*   Updated: 2025/06/04 17:14:32 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* create_cmd:
*	Initialise proprement chaque element de la structure t_cmd.
*	On ne sait pas a l'avance ce que la commande va contenir (args, infile...)
*	C'est pour ça que nous on initialise tous les elements a NULL ou 0 au debut dans create_cmd.
*	Comme ça, le parser va parcourir les tokens et mettre a jour les champs en fonction de ce quil voit.
*	Alloue la memoire dans cmd de taille t_cmd et returne cmd
*/
t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->delimiter = NULL;
	cmd->next = NULL;
	return (cmd);
}

/*
* Rajoute une commande () a la fin de la liste chainée t_cmd *cmds (dans parser)
* La 1er fois comme la liste sera vide en rentre dans le if et on assigne le new_cmd (qui aura comme valeur la structure current dans le parser) a *cmd_list
* La 2eme fois on saute la verification car la liste contiendra la 1ere commande
* et tmp pointera vers la 1ere commande, ensuite comme tmp->next est NULL on rentre pas dans la boucle
* et donc tmp->next pointera maintenant sur la 2eme commande, donc on aura cmd1 -> cmd2 -> NULL;
* et comme ça jusqua la fin des commandes.
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
* on doit l'ajouter à current->args (le tableau de mots de la commande actuelle).
*
* - args est un pointeur vers un tableau de chaînes → char **
* - On doit pouvoir :
*     1) créer ce tableau s'il est vide
*     2) ou l'agrandir s'il existe déjà
*
* Comme on doit modifier le champ `args` (le pointeur lui-même) dans la structure,
* on a besoin de passer un pointeur vers ce pointeur → char ***
*
* C'est pour cela qu'on appelle : add_arg(&current->args, value);
* ce qui fait que add_arg() reçoit un triple pointeur : char ***
*
* Cela permet à add_arg() de faire : *args = nouveau_tableau
* et donc de mettre à jour le champ `args` directement dans t_cmd.
*/

int	add_arg(char ***args, const char *value)
{
	char	**new_args;
	int		count;
	int		i;

	count = 0;
	if (*args)
	{
		while ((*args)[count])
			count++;
	}
	new_args = malloc(sizeof(char *) * (count + 2)); // count → nombre mots dans le tableau; +1 → pour le nouveau qu'on rajoute ; +1 pour NULL;
	if (!new_args)
		return (0);
	i = 0;
	while (i < count)
	{
		new_args[i] = (*args)[i];
		i++;
	}
	new_args[i] = ft_strdup(value);
	new_args[i + 1] = NULL;
	(*args) = new_args;
	return (1);
}
