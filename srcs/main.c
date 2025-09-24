/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 22:54:25 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/22 22:54:46 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define PROMPT "\001\033[32m\002minishell> \001\033[0m\002"

static int	assignments_only(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type != WORD || !is_simple_assignment(tokens->value))
			return (0);
		tokens = tokens->next;
	}
	return (1);
}

static t_token	*discard_assignment_prefix(t_token *tokens, t_pool *pool)
{
	t_token	*next;

	while (tokens && tokens->type == WORD
		&& is_simple_assignment(tokens->value))
	{
		next = tokens->next;
		pool_free_ctx(pool, tokens->value);
		pool_free_ctx(pool, tokens);
		tokens = next;
	}
	return (tokens);
}

static int	store_only_assignments(t_token *tokens, t_expander *exp)
{
	t_token	*iter;

	iter = tokens;
	while (iter)
	{
		if (add_env_variable(exp->pool, &exp->local_env, iter->value))
		{
			free_tokens(exp->pool, tokens);
			exp->last_status = 1;
			return (-1);
		}
		iter = iter->next;
	}
	free_tokens(exp->pool, tokens);
	return (1);
}

static int	handle_assignments(t_token **tokens, t_expander *exp)
{
	int	state;

	if (assignments_only(*tokens))
	{
		state = store_only_assignments(*tokens, exp);
		*tokens = NULL;
		return (state);
	}
	*tokens = discard_assignment_prefix(*tokens, exp->pool);
	if (!*tokens)
		return (1);
	return (0);
}

/*
** Pilote la construction du pipeline: vérifie syntaxe, expansions et heredocs.
*/
static void	run_pipeline(t_token *tokens, char *line, t_expander *exp)
{
	t_cmd	*cmds;

	if (check_syntax_errors(tokens, line))
	{
		exp->last_status = 2;
		return ;
	}
	if (!expand_tokens(tokens, exp))
	{
		exp->last_status = 2;
		return ;
	}
	cmds = parser(tokens, exp);
	if (cmds && handle_heredocs(cmds, exp))
		execute(cmds, exp);
	else
	{
		if (!cmds)
			exp->last_status = 1;
		else if (exp->last_status == 0)
			exp->last_status = 1;
	}
	free_cmds(exp->pool, cmds);
}

static int	process_tokens(char *line, t_expander *exp)
{
	t_token	*tokens;
	int		state;

	tokens = lexer(line, exp);
	if (!tokens)
		return (1);
	state = handle_assignments(&tokens, exp);
	if (state != 0)
		return (1);
	run_pipeline(tokens, line, exp);
	free_tokens(exp->pool, tokens);
	return (1);
}

static int	process_segment(char *line, size_t start, size_t len,
				t_expander *exp)
{
	char	*slice;

	if (len == 0)
		return (1);
	slice = pool_substr_ctx(exp->pool, line, start, len);
	if (!slice)
		return (0);
	process_tokens(slice, exp);
	pool_free_ctx(exp->pool, slice);
	return (1);
}

static int	process_multiline(char *line, t_expander *exp)
{
	size_t	i;
	size_t	start;

	i = 0;
	start = 0;
	while (line[i])
	{
		if (line[i] == '\n')
		{
			if (!process_segment(line, start, i - start, exp))
				return (0);
			start = i + 1;
		}
		i++;
	}
	if (!process_segment(line, start, i - start, exp))
		return (0);
	return (1);
}

static int	handle_interrupted_read(t_expander *exp)
{
	if (g_signal != SIGINT)
		return (0);
	exp->last_status = 130;
	g_signal = 0;
	return (1);
}

static void	track_line_or_exit(char *line, t_expander *exp)
{
	if (pool_track_ctx(exp->pool, line))
		return ;
	free(line);
	error_exit(exp->pool, exp->my_env, "shell_loop: track line");
}

static void	process_non_empty(char *line, t_expander *exp)
{
	add_history(line);
	process_multiline(line, exp);
}

static int	shell_loop(t_expander *exp)
{
	char	*line;

	while (1)
	{
		g_signal = 0;
		line = readline(PROMPT);
		if (!line)
		{
			if (handle_interrupted_read(exp))
				continue ;
			break ;
		}
		track_line_or_exit(line, exp);
		if (line[0])
			process_non_empty(line, exp);
		pool_free_ctx(exp->pool, line);
	}
	return (0);
}

static void	init_shell(t_expander *exp, t_pool *global, char **envp)
{
	pool_init(global);
	exp->pool = global;
	exp->my_env = init_env(envp, global);
	exp->local_env = NULL;
	exp->last_status = 0;
}

int	main(int ac, char **av, char **envp)
{
	t_expander	exp;
	t_pool		global;

	(void)av;
	if (ac != 1)
		return (1);
	init_shell(&exp, &global, envp);
	setup_signals();
	if (!shell_loop(&exp))
	{
		pool_close_all(&global);
		pool_clear(&global);
		rl_clear_history();
		return (0);
	}
	pool_close_all(&global);
	pool_clear(&global);
	return (0);
}
