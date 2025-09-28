/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 22:54:25 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/26 12:57:53 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define PROMPT "\001\033[32m\002minishell> \001\033[0m\002"

static void	init_shell(t_expander *exp, t_pool *global, char **envp)
{
	pool_init(global);
	exp->pool = global;
	exp->my_env = init_env(envp, global);
	exp->local_env = NULL;
	exp->last_status = 0;
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
		pool_free_one(exp->pool, line);
	}
	return (0);
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
