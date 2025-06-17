/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:11:46 by rabiner           #+#    #+#             */
/*   Updated: 2025/06/16 11:13:08 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/wait.h>

# include "../libft/libft.h"

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

/*----------Variable globale-----------*/
// /utils/env.c
extern char	**g_env;// dans main, builtin_1, builtin_utils, utils

/*------Codes ANSI pour formatage------*/
# define YELL	"\033[1;33m"
# define GREE	"\033[1;32m"
# define CYAN	"\033[1;36m"
# define BLUE	"\033[1;34m"
# define MAGE	"\033[1;35m"
# define WHEI	"\033[1;37m"
# define RED	"\033[1;31m"
# define RST	"\033[0m"

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
	char	**g_env;			//	ajouter pour recupere l'env
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

// permet à tous les fichiers (.c) d'accéder à la même variable globale partagée, sans créer
// de duplicata. visible partout mais declaré  dans signals.c
extern volatile sig_atomic_t g_signal;

/*------------------Prototypes------------------*/

/*---------------Commun---------------*/
// /utils/utils.c
void	error_exit(char *str);
void	cleanup_parent(t_cmd *cmd, int *in_fd, int *fd);
void	free_env(void);
void	init_env(char **envp);

/*---------------Signals--------------*/
void	sigint_handler(int sig);
void	sigquit_handler(int sig);
void	setup_signals(void);

/*-------------Execution--------------*/
// /execution/execute.c
void	execute(t_cmd *cmds);
void	execute_command(t_cmd *cmd);

// /execution/redirection.c
void	setup_redirections(t_cmd *cmd, int int_fd, int pipe_fd[2]);

// /execution/check_builtin.c
int		is_builtin(t_cmd *cmd);
int		execute_builtin(t_cmd *cmd);

// /execution/builtin_1.c
int		ft_echo(char **args);
int		ft_cd(char **args);
int		ft_pwd(char **args);
int		ft_env(char **args);

// /execution/builtin_2.c
int		ft_export(char **args);
int		ft_unset(char **args);

// /execution/builtin_utils.c
char	*get_env(char *str);
int		set_env(char *arg);// norminette
void	unset_env(char *arg);

// /execution/path.c
char	*find_command_path(const char *cmd);

/*------------------------------------*/




#endif
