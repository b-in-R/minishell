/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:19:48 by albertooutu       #+#    #+#             */
/*   Updated: 2025/07/14 15:55:21 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
/*
int	main(void)
{
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


void	print_detailled_cmds(t_cmd *cmds)
{
	int	i = 0;

	printf("\n\n### Detailled ###\n");
	while (cmds->args[i])
	{
		printf("args[%i]:\t%s\n", i, cmds->args[i]);
		i++;
	}
	printf("infile:\t\t%s\n", cmds->infile);
	printf("outfile:\t%s\n", cmds->outfile);
	printf("append:\t\t%i\n", cmds->append);
	printf("heredoc:\t%i\n", cmds->heredoc);
	printf("delimiter:\t%s\n", cmds->delimiter);
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
			{
				//printf("\n[%s] ", cmds->args[j]);
				print_detailled_cmds(cmds);
			}
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
*/
// Point d'entrée du programme, gestion de la boucle principale du shell.
/*
* setup_signals(); gere le signaux Ctrl+C et Ctrl+\
* readline();  Affiche le prompt minishell$ attends l'entrée de l'utilisateur, lit l'entrée et  le stocke dans line
*			Comme ca aprés on peut faire le parsing, exec, etc.
* add_history(); // permets d'ajouter line (la commande entrée par le user dans l'histoirque) quand le user a tapé eu moins un caractere
* lexer(); appel le lexer pour identifier chaque token et creer la liste de tokens
* rl_clear_history(); // Vide l'historique de readline avant de quitter
*/

/*
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
*/
/*
	print_tokens(tokens);
	free_tokens(tokens);
*/
/*
 //VERSION FINALE
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

	free_env();
	return (0);
}*/

// precedent main
int	main(int ac, char **av, char **envp)
{
	if (ac == 1)
		(void)av;

	char		*line;
	t_token		*tokens;
	t_cmd		*cmds;
	t_expander	exp;
	int			skip_processing;

	exp.my_env = init_env(envp);
	exp.local_env = NULL;
	exp.last_status = 0;
	setup_signals();
	while (1)
	{
		skip_processing = 0;
		line = readline(GREE"minishell> "RST);
		if (!line)
		{
			printf(BLUE"exit\n"RST);
			rl_clear_history();
			free(line);
			break;
		}
		if (line[0] != '\0')
			add_history(line);

		if (is_simple_assignment(line))
		{
			add_env_variable(&exp.local_env, line);
			skip_processing = 1;
		}
		if (!skip_processing)
		{
			tokens = lexer(line);
			if (!check_syntax_errors(tokens))
			{
				if (expand_tokens(tokens, &exp))
				{
					cmds = parser(tokens);
					if (cmds != NULL)
					{
						if (handle_heredocs(cmds, &exp))
							execute(cmds, NULL, &exp);
						else
							exp.last_status = 1;
						free_cmds(cmds);
					}
					else
						exp.last_status = 2;
				}
				else
					exp.last_status = 2;
			}
			else
				exp.last_status = 2;
			free_tokens(tokens);
		}

		free(line);
	}
	free_env(exp.my_env);
	free_env(exp.local_env);
	return (0);
}
