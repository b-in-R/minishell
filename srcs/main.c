/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:19:48 by albertooutu       #+#    #+#             */
/*   Updated: 2025/06/03 20:38:10 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

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

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_token	*tokens;

	(void)ac;
	(void)av;
	init_env(envp);
	// TEMPORAIRE : appel du lexer + affichage des tokens
	line = "echo \"Hello World\" > file1.txt | wc -l >> result.txt";
	//tokens = lexer(line);
	if (!tokens)
	{
		printf("Erreur : ligne vide ou invalide.\n");
		return (1);
	}
	
/*
	print_tokens(tokens);
	free_tokens(tokens);
*/

/* //VERSION FINALE
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
	//parser
	//executer
	//free_tokens(tokens)
	//free(line); // readline fait malloc donc il faut free
	}
	rl_clear_history();
*/
	free_env();
	return (0);
}

/* precedent main
int	main(void)
{
	char	*line;

	setup_signals(); /// gere le signaux Ctrl+C et Ctrl+\
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
*/
