/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:11:46 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/26 13:02:06 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "mem_manager.h"
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
	t_pool	*pool;
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
	int				leading_space;
	t_pool			*pool;
	struct s_token	*next;
}	t_token;

typedef struct s_lexer_ctx
{
	t_token		**tokens;
	char		*line;
	size_t		*index;
	int			*last_space;
	t_pool		*pool;
}	t_lexer_ctx;

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
void	error_exit(t_pool *pool, char **my_env, const char *str);
void	cleanup_parent(t_pool *pool, t_cmd *cmd, int *in_fd, int *fd);

// /utils/utils.c
void	print_cmds(t_cmd *cmds);
void	print_detailled_cmds(t_cmd *cmds);

/*-------------Execution--------------*/
// /execution/execute.c
void	execute(t_cmd *cmd, t_expander *exp);
void	execute_command(t_cmd *cmd, t_expander *exp);

// /execution/execute_utils.c
void	initialise_data(t_fork *data, t_cmd *cmd, t_expander *exp);
int		count_cmds(t_cmd *cmd);
void	take_exit_code(int *i, int *j, t_fork *data);
void	command_not_found_exit(t_expander *exp, char *cmd);
void	exec_failure_exit(t_expander *exp, char *cmd);

// /execution/redirection.c
void	set_redirection(t_expander *exp, t_cmd *cmd, int in_fd,
			int pipe_fd[2]);

// /builtin/check_builtin.c
int		is_builtin(t_cmd *cmd);
int		execute_builtin(t_cmd *cmd, t_expander *exp);

// /builtin/builtin_1.c
int		ft_echo(char **args);
int		ft_cd(t_expander *exp, char **args);
int		ft_pwd(char **my_env, char **args);
int		has_only_n(char *arg);
int		skip_n_flags(char **args);
int		cd_error(const char *msg, const char *detail);
int		cd_errno(const char *path);
int		cd_set_env(t_expander *exp, const char *key, const char *value);

// /builtin/builtin_2.c
int		ft_export(t_expander *exp, char **args);
int		ft_unset(t_expander *exp, char **args);
int		ft_env(char **my_env);
void	capture_oldpwd(t_expander *exp, char *buf);
int		export_alloc_error(void);
int		export_promote_local(t_expander *exp, const char *name,
			const char *val);
int		export_without_value(t_expander *exp, const char *name);

// /execution/path.c
char	*find_command_path(t_expander *exp, const char *cmd);
char	*join_path(t_pool *pool, const char *dir, const char *cmd);
void	free_split_raw(char **tab, int start);
void	release_tracked_entries(t_pool *pool, char **tab, int count);

// /env/env.c
char	**init_env(char **envp, t_pool *global);
void	free_env(t_pool *pool, char **my_env);
void	print_env(char **env);

// env/env_utils.c
int		set_env(t_pool *pool, char ***env, const char *arg);

// env/env_utils_2.c
char	*get_env(char **my_env, char *str);
void	unset_env(t_pool *pool, char **my_env, char *arg);
int		remove_from_env(t_pool *pool, char **env, const char *key);
int		is_valid_identifier(const char *str);

/*---------------Lexer----------------*/
t_token	*create_token(t_pool *pool, t_token_type type, char *value);
t_token	*lexer(char *line, t_expander *exp);
void	add_token(t_token **list, t_token *new_token);
void	handle_pipe(t_lexer_ctx *ctx);
void	handle_redirection(t_lexer_ctx *ctx);
void	handle_word(t_lexer_ctx *ctx);
void	handle_quotes(t_lexer_ctx *ctx);

/*--------------Parser---------------*/
int		process_token(t_expander *exp, t_token **tokens, t_cmd **current);
t_cmd	*parser(t_token *tokens, t_expander *exp);
t_cmd	*create_cmd(t_expander *exp);
void	add_cmd(t_cmd **cmd_list, t_cmd *new_cmd);
int		add_arg(t_pool *pool, char ***args, const char *value);
void	handle_redirections(t_expander *exp, t_cmd *current, t_token *tokens);
char	*expand_variables(const char *line, t_expander *exp);
int		handle_heredocs(t_cmd *cmds, t_expander *exp);
char	get_unclosed_quote_type(const char *line);
void	handle_redir_in(t_expander *exp, t_cmd *cmd, char *value);
void	handle_redir_out(t_expander *exp, t_cmd *cmd, char *value, int type);
void	handle_heredoc(t_expander *exp, t_cmd *cmd, t_token *tok);
int		has_unclosed_quotes(const char *line);
int		check_syntax_errors(t_token *tokens, char *line);
char	*remove_outer_quotes(t_pool *pool, const char *str);
int		push_line(int write_fd, char *line, t_expander *exp, t_cmd *cmd);
void	handle_pool_track_failure(char *line, t_expander *exp, int write_fd);
char	*expand_heredoc_line(const char *line, t_expander *exp);
int		write_all(int fd, const char *buf, size_t len);
void	heredoc_child_exit(t_expander *exp, int write_fd, int status);

/*--------------Expander--------------*/
int		expand_tokens(t_token *tokens, t_expander *exp);
char	*expand_word(t_pool *pool, const char *word, t_expander *exp);
char	*join_tokens(t_pool *pool, t_token *tokens);
char	*get_env_value_from_exp(const char *key, t_expander *exp);
void	update_quote_flags(char c, int *in_single, int *in_double);
int		exp_variable(t_pool *pool, const char *str, int *i, char **result,
			t_expander *exp);
int		handle_dollar(t_pool *pool, const char *word, int *i, char **result,
			t_expander *exp);
int		append_char(t_pool *pool, char **str, char c);
int		str_append_free(t_pool *pool, char **s1, const char *s2);
int		is_var_char(char c);
int		add_env_variable(t_pool *pool, char ***env, const char *line);
int		is_simple_assignment(const char *line);
int		expand_exit_status(t_pool *pool, char **result,
			t_expander *exp, int *i);
int		read_self_pid(void);
int		expand_pid(t_pool *pool, char **result, int *i);

/*--------------Utils---------------*/
void	free_env(t_pool *pool, char **env);
char	*ft_strjoin_3(t_pool *pool, const char *s1, const char *s2,
			const char *s3);
char	**create_clean_args(t_pool *pool, char **args);
int		assignments_only(t_token *tokens);
t_token	*discard_assignment_prefix(t_token *tokens, t_pool *pool);
int		store_only_assignments(t_token *tokens, t_expander *exp);
int		handle_assignments(t_token **tokens, t_expander *exp);
void	run_pipeline(t_token *tokens, char *line, t_expander *exp);
int		process_tokens(char *line, t_expander *exp);
int		process_segment(char *line, size_t start, size_t len,
			t_expander *exp);
int		process_multiline(char *line, t_expander *exp);
int		handle_interrupted_read(t_expander *exp);
void	track_line_or_exit(char *line, t_expander *exp);
void	process_non_empty(char *line, t_expander *exp);

/*---------------Free----------------*/
void	free_allocs(t_pool *pool, char **tofree);
void	free_tokens(t_pool *pool, t_token *tokens);
void	free_cmds(t_pool *pool, t_cmd *cmds);

/*--------------Signals--------------*/
void	setup_signals(void);
void	sigint_handler(int sig);
void	sigquit_handler(int sig);
void	sigint_heredoc(int sig);

#endif
