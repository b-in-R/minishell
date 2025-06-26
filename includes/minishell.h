/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:11:46 by rabiner           #+#    #+#             */
/*   Updated: 2025/06/26 16:38:43 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/wait.h>

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

/*------Codes ANSI pour formatage------*/
# define YELL	"\033[1;33m"
# define GREE	"\033[1;32m"
# define CYAN	"\033[1;36m"
# define BLUE	"\033[1;34m"
# define MAGE	"\033[1;35m"
# define WHEI	"\033[1;37m"
# define RED	"\033[1;31m"
# define RST	"\033[0m"

// permet à tous les fichiers (.c) d'accéder à la même variable globale partagée, sans créer de duplicata. visible partout mais declaré	dans signals.c
extern volatile sig_atomic_t	g_signal;

/*------------------Prototypes------------------*/

/*---------------Commun---------------*/
// /utils/utils.c
void	error_exit(char **my_env, char *str);
void	cleanup_parent(t_cmd *cmd, int *in_fd, int *fd);

// /srcs/main.c
void	print_cmds(t_cmd *cmds);
void	print_detailled_cmds(t_cmd *cmds);

/*-------------Execution--------------*/
// /execution/execute.c
int		execute(t_cmd *cmds, char **av, char **my_env);
void	execute_command(t_cmd *cmd, char **my_env);

// /execution/redirection.c
void	setup_redirections(char **my_env, t_cmd *cmd, int int_fd, int pipe_fd[2]);

// /builtin/check_builtin.c
int		is_builtin(t_cmd *cmd);
int		execute_builtin(t_cmd *cmd, char **my_env);

// /builtin/builtin_1.c
int		ft_echo(char **args);
int		ft_cd(char **my_env, char **args);
int		ft_pwd(char **my_env, char **args);
int		ft_env(char **my_env, char **args);

// /builtin/builtin_2.c
int		ft_export(char **my_env, char **args);
int		ft_unset(char **my_env, char **args);

// /execution/path.c
char	*find_command_path(char **my_env, const char *cmd);

// /env/env.c
char	**init_env(char **envp);
void	free_env(char **my_env);

// env/env_utils.c
char	*get_env(char **env, char *str);
int		set_env(char **env, char *arg);
void	unset_env(char **env, char *arg);
/*------------------------------------*/



/*---------------Lexer----------------*/
t_token	*lexer(char *line);
t_token	*create_token(t_token_type type, char *value);
void	add_token(t_token **list, t_token *new_token);
void	handle_pipe(t_token **tokens, size_t *i);
void	handle_redirection(t_token **tokens, char *line, size_t *i);
void	handle_word(t_token **tokens, char *line, size_t *i);
void	handle_quotes(t_token **tokens, char *line, size_t *i);

/*--------------Parser---------------*/
t_cmd	*parser(t_token *tokens);
t_cmd	*create_cmd(void);
void	add_cmd(t_cmd **cmd_list, t_cmd *new_cmd);
int		add_arg(char ***args, const char *value);
void	handle_redirections(t_cmd *current, t_token **tokens);
int		check_syntax_errors(t_token *tokens);

/*--------------Expander--------------*/
void	expand_tokens(t_token *tokens, int last_status);
char	*expand_word(const char *word, int last_status);
char	*get_env_value(const char *word);
void	update_quote_flags(char c, int *in_single, int *in_double);
void	handle_dollar(const char *word, int *i, char **result, int last_status);
void	append_char(char **str, char c);
char	*str_append_free(char *s1, const char *s2);

/*--------------Utils---------------*/
void	free_tokens(t_token *tokens);
void	free_cmds(t_cmd *cmds);

#endif
