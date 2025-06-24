/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:19:48 by albertooutu       #+#    #+#             */
/*   Updated: 2025/06/24 15:31:33 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*cmds;
	int		last_status; // Code de retour de la dernière commande exécutée. (pour traiter $? dans l'expansion)

	last_status = 0; // Initialisation du dernier statut à 0 (succès)
	setup_signals(); // Gère les signaux (Ctrl-C, Ctrl-\), avant la boucle parce que readline() va attendre l'entrée de l'utilisateur et on veut que les signaux soient gérés correctement.
	while (1)
	{
	line = readline(GREE"minishell> "RST);
	if (!line)
	{
		printf(BLUE"exit\n"RST);
		rl_clear_history();
		return (0); // Si readline retourne NULL, c'est qu'on a tapé Ctrl-D (EOF), on quitte le shell
	}
	if (line[0] != '\0')
		add_history(line);
	tokens = lexer(line);
	if (!check_syntax_errors(tokens))
	{
		if (expand_tokens(tokens, last_status))
		{
			cmds = parser(tokens);
			if (cmds != NULL)
			{
				if (handle_heredocs(cmds))
					last_status = execute(cmds);
				else
					last_status = 1;
				free_cmds(cmds);
			}
			else
				last_status = 2;
		}
		else
			last_status = 2; // échec de l'expansion
	}
	else
		last_status = 2; // Si il y a une erreur de syntaxe, on met last_status à 2
	free_tokens(tokens);
	free(line); // readline fait malloc donc il faut free
	}
	return (0);
}
