/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:19:48 by albertooutu       #+#    #+#             */
/*   Updated: 2025/09/10 12:36:30 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
* setup_signals(); Handles the Ctrl+C and Ctrl+\ signals
* readline(); Displays the minishell prompt and waits for user input
* Reads the input and stores it in line
* ​​So we can then perform parsing, exec, etc.
* When the user has typed at least one character
* lexer();calls lexer to tokenize the input line into a linked list of t_token
* rl_clear_history(); // Vide l'historique de readline avant de quitter
*/
int	process_input(char *line, t_expander *exp)
{
	t_token		*tokens;
	t_cmd		*cmds;

	if (is_simple_assignment(line))
		return (add_env_variable(&exp->local_env, line), 0);
	tokens = lexer(line);
	if (!check_syntax_errors(tokens, line))
	{
		if (expand_tokens(tokens, exp))
		{
			cmds = parser(tokens);
			if (cmds && handle_heredocs(cmds, exp))
				execute(cmds, exp);
			else
				exp->last_status = 1;
			free_cmds(cmds);
		}
		else
			exp->last_status = 2;
	}
	else
		exp->last_status = 2;
	free_tokens(tokens);
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	t_expander	exp;
	char		*line;

	(void)av;
	if (ac != 1)
		return (1);
	exp.my_env = init_env(envp);
	exp.local_env = NULL;
	exp.last_status = 0;
	setup_signals();
	while (1)
	{
		line = readline("\001\033[32m\002minishell> \001\033[0m\002");
		if (!line)
			return (rl_clear_history(), 0);
		if (line[0] != '\0')
		{
			add_history(line);
			process_input(line, &exp);
		}
		free(line);
	}
	return (0);
}
