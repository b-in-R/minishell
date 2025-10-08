/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 22:54:25 by rabiner           #+#    #+#             */
/*   Updated: 2025/10/08 12:47:35 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	init_shell(t_expander *exp, t_pool *global, char **envp)
{
	pool_init(global);
	exp->pool = global;
	exp->my_env = init_env(envp, global);
	exp->local_env = NULL;
	exp->last_status = 0;
}

void	track_line_or_exit(char *line, t_expander *exp)
{
	if (pool_track(exp->pool, line))
		return ;
	free(line);
	error_exit(exp->pool, exp->my_env, "shell_loop: track line");
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
		{
			add_history(line);
			process_multiline(line, exp);
		}
		//	process_non_empty(line, exp);
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
