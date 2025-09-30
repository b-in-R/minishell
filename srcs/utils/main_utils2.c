/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:09:09 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/26 12:58:17 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	process_multiline(char *line, t_expander *exp)
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

int	handle_interrupted_read(t_expander *exp)
{
	if (g_signal != SIGINT)
		return (0);
	exp->last_status = 130;
	g_signal = 0;
	return (1);
}

void	track_line_or_exit(char *line, t_expander *exp)
{
	if (pool_track(exp->pool, line))
		return ;
	free(line);
	error_exit(exp->pool, exp->my_env, "shell_loop: track line");
}

void	process_non_empty(char *line, t_expander *exp)
{
	add_history(line);
	process_multiline(line, exp);
}
