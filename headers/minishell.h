/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:11:46 by rabiner           #+#    #+#             */
/*   Updated: 2025/05/23 13:15:03 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "rabiner_inputs.h"

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
}  	t_token_type;

/* Represents a single token parsed from the input line
*	t_token are used to analyze and understand what the user wants to do, step by step.
*/
typedef struct s_token {
	char			*value; // The actual content of the token (e.g. "ls", ">", "file.txt")
	t_token_type	type;   // The type of this token (WORD, PIPE, etc.)
	struct s_token	*next;  // Pointer to the next token in the list
} t_token;

/* EXAMPLE:
* 	a command line like this:  "ls -l | grep minishell > output.txt"
* 	it will be transformed in a t_token list like this:
* 	["ls", WORD] → ["-l", WORD] → ["|", PIPE] → ["grep", WORD] → ["minishell", WORD] → [">", REDIR_OUT] → ["output.txt", WORD]
*/


/*-------------Prototypes-------------*/

#endif
