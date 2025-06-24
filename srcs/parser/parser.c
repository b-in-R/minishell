/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:11:41 by albertooutu       #+#    #+#             */
/*   Updated: 2025/06/24 15:29:43 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
*	Parser:
*	Reads the list of t_token (tokens) to build structured t_cmd elements
*	(complete commands to execute, every time we find a PIPE (|) we create a t_cmd to execute).
*
*	"Étape"							"Que fait-on ?"
*	- Début							current == NULL, donc on crée une t_cmd vide
*	- Tant qu'on lit un token		On met à jour le champ correspondant de current
*	- Dès qu'on rencontre un PIPE	On remet current = NULL → une nouvelle commande sera créée à la prochaine itération
*	- À chaque fin de boucle		On avance au token suivant (tokens = tokens->next)
*
*/
/*
*	commande:		echo hello > out.txt | grep test < file.txt
*	list tokens:	[WORD: echo] → [WORD: hello] → [REDIR_OUT: >] → [WORD: out.txt] → [PIPE: |] → [WORD: grep] → [WORD: test] → [REDIR_IN: <] → [WORD: file.txt]
*	Résultat final : Deux commandes t_cmd
*					- commande 1: args = {"echo", "hello"} → outfile = "out.txt" → append = 0
*					- commande 2: args = {"grep", "test"} → infile = "file.txt"
*
*/
t_cmd	*parser(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*current;
	char	*clean;

	cmds = NULL;
	current = NULL;
	while (tokens)
	{
		if (current == NULL)
		{
			current = create_cmd();
			add_cmd(&cmds, current);
		}
		if (tokens->type == WORD)
		{
			clean = remove_outer_quotes(tokens->value);
			add_arg(&current->args, clean);
			free(clean);
			tokens = tokens->next;
		}
		else if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT
			|| tokens->type == REDIR_APPEND || tokens->type == HEREDOC)
		{
			if (tokens->next == NULL || tokens->next->type != WORD)
			{
				printf("Syntax error: expected filename after redirection\n");
				return (free_cmds(cmds), NULL);
			}
			handle_redirections(current, tokens);
			tokens = tokens->next->next;
		}
		else if (tokens->type == PIPE)
		{
			current = NULL;
			tokens = tokens->next;
		}
		else
			tokens = tokens->next;
	}
	return (cmds);
}

char	*remove_outer_quotes(const char *str)
{
	size_t	len;
	char	*res;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len < 2)
		return (ft_strdup(str));
	if ((str[0] == '\'' && str[len - 1] == '\'') ||
		(str[0] == '"' && str[len - 1] == '"'))
	{
		res = ft_substr(str, 1, len - 2); // supprime les quotes autour
	}
	else
		res = ft_strdup(str);
	return (res);
}

/*
*	Pour racourcir la fonction parser, mets a jour les valeurs des champs correspondants
*/
void	handle_redirections(t_cmd *current, t_token *tokens)
{
	if (tokens->type == REDIR_IN)
		current->infile = ft_strdup(tokens->value);
	else if (tokens->type == REDIR_OUT)
	{
		current->outfile = ft_strdup(tokens->value);
		current->append = 0;
	}
	else if (tokens->type == REDIR_APPEND)
	{
		current->outfile = ft_strdup(tokens->value);
		current->append = 1;
	}
	else if (tokens->type == HEREDOC)
	{
		current->heredoc = 1;
		current->delimiter = remove_outer_quotes(tokens->next->value);
	}
}

/*
*	Lit l'entrée utilisateur ligne par ligne jusqu'à ce que le délimiteur soit atteint.
*/
static int	read_heredoc(const char *delimiter, int pipe_fd[2])
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	return (0);
}
// ???? gérer les quotes dans le délimiteur (ex : "<< 'EOF'")?????? A Implementer
// ???? Gerer les signaux en cas dinterruption (Ctrl-C) pendant la lecture de l'heredoc ????
/*  Appel par le main avant l'exécution des commandes et aprés le parsing.
*	Parcours chaque t_cmd dans ta liste.
*	Si détecte que cmd->heredoc == 1, alors on:
*		- Lit l’entrée utilisateur ligne par ligne jusqu’au délimiteur.
*		- On stockes ça dans un pipe temporaire.
*		- On ouvre ce pipe/fichier en lecture, et on mets son fd dans cmd->in_fd.
*	Comem ca lors de l’exécution (execute()), le shell saura que l’entrée (stdin) de cette commande ne doit pas venir du clavier, mais de ce contenu temporaire.
*/
int	handle_heredocs(t_cmd *cmds)
{
	int		pipe_fd[2];

	while (cmds)
	{
		if (cmds->heredoc && cmds->delimiter)
		{
			if (pipe(pipe_fd) == -1)
				return (perror("pipe"), 0);
			if (read_heredoc(cmds->delimiter, pipe_fd) == -1)
				return (0);
			cmds->infile = NULL;
			cmds->in_fd = pipe_fd[0];
		}
		cmds = cmds->next;
	}
	return (1);
}


