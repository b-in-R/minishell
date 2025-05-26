/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:19:48 by albertooutu       #+#    #+#             */
/*   Updated: 2025/05/26 12:17:31 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

// Point d'entrée du programme, gestion de la boucle principale du shell.
int	main(void)
{
	char	*line;

	setup_signals(); /*gere le signaux Ctrl+C et Ctrl+\*/
	while (1)
	{
	// Affiche le prompt minishell$ attends l'entrée de l'utilisateur, lit l'entrée et  le stocke dans line
	// Comme ca aprés on peut faire le parsing, exec, etc.
	line = readline("minishell$ ");
	if (!line)
	{
		printf("exit\n"); // Affiche exit comme bash (Ctrl-D)
		break;
	}
	if (line[0] != '\0')
		add_history(line);// permets d'ajouter line (la commande entrée par le user dans l'histoirque) quand le user a tapé eu moins un caractere
	free(line); // readline fait malloc donc il faut free
	}
	return (0);
}
