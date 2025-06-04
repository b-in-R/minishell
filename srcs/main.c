/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:19:48 by albertooutu       #+#    #+#             */
/*   Updated: 2025/06/04 18:43:08 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Point d'entrée du programme, gestion de la boucle principale du shell.
/*
* setup_signals(); gere le signaux Ctrl+C et Ctrl+\
* readline();  Affiche le prompt minishell$ attends l'entrée de l'utilisateur, lit l'entrée et  le stocke dans line
*			Comme ca aprés on peut faire le parsing, exec, etc.
* add_history(); // permets d'ajouter line (la commande entrée par le user dans l'histoirque) quand le user a tapé eu moins un caractere
* lexer(); appel le lexer pour identifier chaque token et creer la liste de tokens
* rl_clear_history(); // Vide l'historique de readline avant de quitter
*/
//TEMPORAIRE: affichage types
char	*token_type_str(t_token_type type)
{
	if (type == WORD) return "WORD";
	if (type == PIPE) return "PIPE";
	if (type == REDIR_IN) return "REDIR_IN";
	if (type == REDIR_OUT) return "REDIR_OUT";
	if (type == REDIR_APPEND) return "REDIR_APPEND";
	if (type == HEREDOC) return "HEREDOC";
	return "UNKNOWN";
}

//TEMPORAIRE : fonction de debug pour afficher les tokens
void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Type: %s | Value: \"%s\"\n", token_type_str(tokens->type), tokens->value);
		tokens = tokens->next;
	}
}

//TEMPORAIRE : fonction pour montrer les cmds
void print_cmds(t_cmd *cmds)
{
	int	i = 1;
	while (cmds)
	{
		printf("-- CMD %d --\n", i++);
		if (cmds->args)
		{
			printf("Args: ");
			for (int j = 0; cmds->args[j]; j++)
				printf("[%s] ", cmds->args[j]);
			printf("\n");
		}
		if (cmds->infile)
			printf("Infile: %s\n", cmds->infile);
		if (cmds->outfile)
			printf("Outfile: %s (%s)\n", cmds->outfile, cmds->append ? "append" : "truncate");
		if (cmds->heredoc)
			printf("Heredoc delimiter: %s\n", cmds->delimiter);
		cmds = cmds->next;
	}
}

int	main(void)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*cmds;

	// TEMPORAIRE: pour tester
	line = "cat < input.txt | wc -l >> result.txt";
	printf("==> Ligne testée : %s\n\n", line);
	tokens = lexer(line);
	if (!tokens)
	{
		printf("Erreur : ligne vide ou invalide.\n");
		return (1);
	}
	cmds = parser(tokens);
	printf("=== TOKENS ===\n");
	print_tokens(tokens);
	printf("\n=== PARSED COMMANDS ===\n");
	print_cmds(cmds);
	free_tokens(tokens);
	free_cmds(cmds);
/* VERSION FINALE (FAUT COMPLETER AU FUR A MESURE)
	while (1)
	{
	line = readline("minishell> ");
	if (!line)
	{
		printf("exit\n");
		break;
	}
	if (line[0] != '\0')
		add_history(line);
	tokens = lexer(line);
	cmds = parser(tokens);
	//executer
	free_tokens(tokens);
	free_cmds(cmds);
	free(line); // readline fait malloc donc il faut free
	}
	rl_clear_history();
*/
	return (0);
}
