/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:11:46 by rabiner           #+#    #+#             */
/*   Updated: 2025/05/23 14:53:51 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "rabiner_inputs.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

/*	Fonctions autorisees

	access
	add_history
	chdir
	close
	dup, dup2
	execve
	exit
	fork
	free
	getcwd, getenv
	isatty, ttyname, ttyslot
	ioctl
	kill
	malloc
	open
	opendir, closedir
	pipe
	printf
	read
	readline
	rl_clear_history
	rl_on_new_line
	rl_redisplay
	rl_replace_line
	sigaction, sigaddset, sigemptyset, signal
	stat, lstat, fstat
	strerror, perror
	tcsetattr, tcgetattr
	tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
	unlink
	wait, waitpid, wait3, wait4
	write
*/
/*-------------Structures--------------*/
// Token types recognized in the input line
typedef enum	e_token_type
{
	WORD,         // A simple word (ex: ls, echo, etc. command or argument)
	PIPE,         // Pipe symbol '|' to chain commands
	REDIR_IN,     // Input redirection symbol '<'
	REDIR_OUT,    // Output redirection symbol '>'
	REDIR_APPEND, // Output append redirection symbol '>>'
	HEREDOC       // Here-document symbol '<<' for multiline input
}	t_token_type;

/* Represents a single token parsed from the input line
*	t_token are used to analyze and understand what the user wants to do, step by step.
*/
typedef struct	s_token {
	char			*value; // The actual content of the token (e.g. "ls", ">", "file.txt")
	t_token_type	type;   // The type of this token (WORD, PIPE, etc.)
	struct s_token	*next;  // Pointer to the next token in the list
}	t_token;

/* EXAMPLE:
* 	a command line like this:  "ls -l | grep minishell > output.txt"
* 	it will be transformed in a t_token list like this:
* 	["ls", WORD] → ["-l", WORD] → ["|", PIPE] → ["grep", WORD] → ["minishell", WORD] → [">", REDIR_OUT] → ["output.txt", WORD]
*/

// Represents a complete shell command with its arguments and redirections
typedef struct	s_cmd {
	char	**args;         // The list of arguments for the command, including the command itself
	char	*infile;        // Input redirection file (for '<')
	char	*outfile;       // Output redirection file (for '>' or '>>')
	int		append;          // 1 if '>>' (append mode), 0 if '>'
	int		heredoc;         // 1 if '<<' is used (heredoc), 0 otherwise
	char	*delimiter;     // The heredoc delimiter string (after '<<')
	struct	s_cmd*next;  // Pointer to the next command in a pipeline (after '|')
}	t_cmd;

/* EXAMPLE:
*t_token
*	Sert à analyser la ligne de commande du user mot par mot.
*	Chaque mot (commande, argument, symbole comme |, <, etc.) est un token.
*	Ça nous aide à comprendre l’intention du user.
*		→ C’est l’analyse grammaticale (comme découper une phrase en mots et types).
*
*t_cmd
*	Sert à organiser toutes ces infos pour l’exécution réelle.
*	Une fois les tokens identifiés, on les regroupe :
*	Les arguments vont ensemble (ls, -l)
*	On stocke les redirections (ex: > ou >>)
*	On relie les commandes entre elles si | est utilisé
*		→ C’est la construction du sens complet, prête à être exécutée
*
*	Transition : de t_token vers t_cmd dans le parser
*	Ex : ligne entrée par l’utilisateur:
*		"ls -l | grep hello > out.txt"
*
*	Étape 1 : Le lexer produit cette liste de tokens :
*		["ls", WORD] → ["-l", WORD] → ["|", PIPE] → ["grep", WORD] → ["hello", WORD] → [">", REDIR_OUT] → ["out.txt", WORD]
*
*	Étape 2 : Le parser lit cette liste et construit deux t_cmd
*		Première commande : ls -l
*		t_cmd *cmd1;
*		cmd1->args = {"ls", "-l", NULL};
*		cmd1->outfile = NULL;
*		cmd1->next = cmd2;
*
*		Deuxième commande : grep hello > out.txt
*		t_cmd *cmd2;
*		cmd2->args = {"grep", "hello", NULL};
*		cmd2->append = 0;
*		cmd2->next = NULL;
*
*	Conclusion :
*		On analyse (avec t_token) pour comprendre l'entrée du user.
*		On organise (avec t_cmd) pour préparer l'exécution.
*		Grâce à ça, on pourra appeler une fonction execute(t_cmd *cmds) et faire tourner tout le shell.
*/

/*-------------Prototypes-------------*/

#endif
