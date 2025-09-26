/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:02:57 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/26 12:41:27 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*join_path(t_pool *pool, const char *dir, const char *cmd)
{
	char	*path;
	char	*tmp;

	tmp = pool_strjoin_ctx(pool, dir, "/");
	if (!tmp)
		return (NULL);
	path = pool_strjoin_ctx(pool, tmp, cmd);
	if (!path)
	{
		pool_free_ctx(pool, tmp);
		return (NULL);
	}
	pool_free_ctx(pool, tmp);
	return (path);
}

void	free_split_raw(char **tab, int start)
{
	int	idx;

	idx = start;
	while (tab && tab[idx])
	{
		free(tab[idx]);
		idx++;
	}
}

void	release_tracked_entries(t_pool *pool, char **tab, int count)
{
	int	idx;

	idx = count;
	while (idx > 0)
	{
		idx--;
		pool_free_ctx(pool, tab[idx]);
	}
}
