/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:11:46 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/10 12:42:02 by albertooutu      ###   ########.fr       */
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
/* Token types recognized in the input line
* 	WORD,         // A simple word (ex: ls, echo, etc. command or argument)
	PIPE,         // Pipe symbol '|' to chain commands
	REDIR_IN,     // Input redirection symbol '<'
	REDIR_OUT,    // Output redirection symbol '>'
	REDIR_APPEND, // Output append redirection symbol '>>'
	HEREDOC       // Here-document symbol '<<' for multiline input
*/
typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC,
}	t_token_type;

typedef struct s_expander
{
	int		last_status;
	char	**my_env;
	char	**local_env;
}	t_expander;

/*
*	Quote types used to handle quotes in the input line
*/
typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}	t_quote_type;

/* Represents a single token parsed from the input line
*	t_token are used to analyze and understand what the user wants to do.
*	char *value; //The actual content of the token (e.g. "ls", ">", "file.txt")
*	t_token_type	type;   // The type of this token (WORD, PIPE, etc.)
*	t_quote_type	quoted_type; // Type of quotes (SINGLE, DOUBLE, NONE)
*	struct s_token	*next;  // Pointer to the next token in the list
*/
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	t_quote_type	quoted_type;
	struct s_token	*next;
}	t_token;

/* Represents a complete shell command with its arguments and redirections
*  	char	**args;    // list of arguments for the command
*	char	*infile;   // Input redirection file (for '<')
*	char	*outfile;  // Output redirection file (for '>' or '>>')
*	int		append;    // 1 if '>>' (append mode), 0 if '>'
*	int		heredoc;   // 1 if '<<' is used (heredoc), 0 otherwise
*	int		expand_heredoc; // 1 si on fait l’expansion, 0 sinon
*	int		in_fd;	   // File descriptor for input redirection (if any)
*	char	*delimiter; // The heredoc delimiter string (after '<<')
*	struct	s_cmd*next; // Pointer to the next command in a pipeline (after '|')
*/
typedef struct s_cmd
{
	char			**args;
	char			*infile;
	char			*outfile;
	int				append;
	int				heredoc;
	int				expand_heredoc;
	int				in_fd;
	char			*delimiter;
	struct s_cmd	*next;
}	t_cmd;

/*
*	Struct for execute.c
*/
typedef struct s_fork
{
	int		fd[2];
	int		in_fd;
	pid_t	*pid;
	int		status;
	int		last_status;
}	t_fork;

/*------Codes ANSI pour formatage------*/
# define YELL	"\033[1;33m"
# define GREE	"\033[1;32m"
# define CYAN	"\033[1;36m"
# define BLUE	"\033[1;34m"
# define MAGE	"\033[1;35m"
# define WHEI	"\033[1;37m"
# define RED	"\033[1;31m"
# define RST	"\033[0m"

/*	allows all files (.c) to access the same shared global variable,
*	without creating a duplicate. visible everywhere but declared in signals.c
*/
extern volatile sig_atomic_t	g_signal;

/*------------------Prototypes------------------*/

/*---------------Common---------------*/
// /utils/utils.c
void	error_exit(char **my_env, char *str);
void	cleanup_parent(t_cmd *cmd, int *in_fd, int *fd);

// /srcs/main.c
void	print_cmds(t_cmd *cmds);
void	print_detailled_cmds(t_cmd *cmds);

/*-------------Execution--------------*/
// /execution/execute.c
void	execute(t_cmd *cmd, t_expander *exp);
void	execute_command(t_cmd *cmd, char **my_env);

// /execution/execute_utils.c
void	initialise_data(t_fork *data, t_cmd *cmd);
int		count_cmds(t_cmd *cmd);

// /execution/redirection.c
void	set_redirection(char **my_env, t_cmd *cmd, int int_fd, int pipe_fd[2]);

// /builtin/check_builtin.c
int		is_builtin(t_cmd *cmd);
int		execute_builtin(t_cmd *cmd, t_expander *exp);

// /builtin/builtin_1.c
int		ft_echo(char **args);
int		ft_cd(char **my_env, char **args);
int		ft_pwd(char **my_env, char **args);

// /builtin/builtin_2.c
int		ft_export(t_expander *exp, char **args);
int		ft_unset(t_expander *exp, char **args);
int		ft_env(char **my_env);

// /execution/path.c
char	*find_command_path(char **my_env, const char *cmd);

// /env/env.c
char	**init_env(char **envp);
void	free_env(char **my_env);
void	print_env(char **env);

// env/env_utils.c
char	**set_env(char **env, const char *arg);

// env/env_utils_2.c
char	*get_env(char **my_env, char *str);
void	unset_env(char **my_env, char *arg);
int		remove_from_env(char **env, const char *key);
int		is_valid_identifier(const char *str);

/*---------------Lexer----------------*/
t_token	*create_token(t_token_type type, char *value);
t_token	*lexer(char *line);
void	add_token(t_token **list, t_token *new_token);
void	handle_pipe(t_token **tokens, size_t *i);
void	handle_redirection(t_token **tokens, char *line, size_t *i);
void	handle_word(t_token **tokens, char *line, size_t *i);
void	handle_quotes(t_token **tokens, char *line, size_t *i);

/*--------------Parser---------------*/
int		process_token(t_token **tokens, t_cmd **current);
t_cmd	*parser(t_token *tokens);
t_cmd	*create_cmd(void);
void	add_cmd(t_cmd **cmd_list, t_cmd *new_cmd);
int		add_arg(char ***args, const char *value);
void	handle_redirections(t_cmd *current, t_token *tokens);
char	*expand_variables(const char *line, t_expander *exp);
int		handle_heredocs(t_cmd *cmds, t_expander *exp);
char	get_unclosed_quote_type(const char *line);
int		has_unclosed_quotes(const char *line);
int		check_syntax_errors(t_token *tokens, char *line);
char	*remove_outer_quotes(const char *str);

/*--------------Expander--------------*/
int		expand_tokens(t_token *tokens, t_expander *exp);
char	*expand_word(const char *word, t_expander *exp);
char	*join_tokens(t_token *tokens);
char	*get_env_value_from_exp(const char *key, t_expander *exp);
void	update_quote_flags(char c, int *in_single, int *in_double);
int		exp_variable(const char *str, int *i, char **result, t_expander *exp);
int		handle_dollar(const char *word, int *i, char **result, t_expander *exp);
int		append_char(char **str, char c);
int		str_append_free(char **s1, const char *s2);
int		is_var_char(char c);
int		add_env_variable(char ***env, const char *line);
int		is_simple_assignment(const char *line);

/*--------------Utils---------------*/
void	free_tokens(t_token *tokens);
void	free_cmds(t_cmd *cmds);
void	free_env(char **env);
char	*ft_strjoin_3(char *s1, char *s2, char *s3);
char	**create_clean_args(char **args);

/*--------------Signals--------------*/
void	setup_signals(void);
void	sigint_handler(int sig);
void	sigquit_handler(int sig);
void	sigint_heredoc(int sig);

#endif
