/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:43:15 by albertooutu       #+#    #+#             */
/*   Updated: 2025/09/26 12:34:26 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
* Gère l'expansion de $? (exit status)
*/
int	expand_exit_status(t_pool *pool, char **result,
		t_expander *exp, int *i)
{
	char	*value;

	value = ft_itoa(exp->last_status);
	if (!value)
		return (0);
	if (!pool_track_ctx(pool, value))
	{
		free(value);
		return (0);
	}
	if (!str_append_free(pool, result, value))
		return (pool_free_ctx(pool, value), 0);
	pool_free_ctx(pool, value);
	(*i)++;
	return (1);
}

int	read_self_pid(void)
{
	int		fd;
	ssize_t	len;
	char	buf[32];
	int		idx;

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
		return (-1);
	len = read(fd, buf, sizeof(buf) - 1);
	close(fd);
	if (len <= 0)
		return (-1);
	buf[len] = '\0';
	idx = 0;
	while (buf[idx] && buf[idx] != ' ')
		idx++;
	buf[idx] = '\0';
	return (ft_atoi(buf));
}

int	expand_pid(t_pool *pool, char **result, int *i)
{
	char	*value;
	int		pid;

	pid = read_self_pid();
	if (pid < 0)
		value = ft_strdup("0");
	else
		value = ft_itoa(pid);
	if (!value)
		return (0);
	if (!pool_track_ctx(pool, value))
	{
		free(value);
		return (0);
	}
	if (!str_append_free(pool, result, value))
		return (pool_free_ctx(pool, value), 0);
	pool_free_ctx(pool, value);
	(*i)++;
	return (1);
}
