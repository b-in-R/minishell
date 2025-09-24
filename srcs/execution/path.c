/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:02:57 by rabiner           #+#    #+#             */
/*   Updated: 2025/09/01 15:46:14 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*join_path(t_pool *pool, const char *dir, const char *cmd)
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

static void	free_split_raw(char **tab, int start)
{
	int	idx;

	idx = start;
	while (tab && tab[idx])
	{
		free(tab[idx]);
		idx++;
	}
}

static void	release_tracked_entries(t_pool *pool, char **tab, int count)
{
	int	idx;

	idx = count;
	while (idx > 0)
	{
		idx--;
		pool_free_ctx(pool, tab[idx]);
	}
}

// Adds every element of a split PATH array to the pool tracking.
static int	track_split(t_pool *pool, char **tab)
{
	int	i;

	if (!tab)
		return (1);
	if (!pool_track_ctx(pool, tab))
	{
		free_split_raw(tab, 0);
		free(tab);
		return (0);
	}
	i = 0;
	while (tab[i])
	{
		if (!pool_track_ctx(pool, tab[i]))
		{
			free_split_raw(tab, i);
			release_tracked_entries(pool, tab, i);
			pool_free_ctx(pool, tab);
			return (0);
		}
		i++;
	}
	return (1);
}

// Frees a previously tracked split array using the pool helpers.
static void	free_split_except(t_pool *pool, char **tab, char *keep)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		if (tab[i] != keep)
			pool_free_ctx(pool, tab[i]);
		i++;
	}
	pool_free_ctx(pool, tab);
}

static char	*cleanup_and_return(t_pool *pool, char **paths, char *keep,
			char *value)
{
	free_split_except(pool, paths, keep);
	return (value);
}

// Iterates PATH entries to locate an executable with X_OK permission.
static char	*find_command_path_2(t_pool *pool, const char *cmd,
			char **paths)
{
	int		i;
	char	*path;
	char	*candidate;

	i = 0;
	candidate = NULL;
	while (paths[i])
	{
		path = join_path(pool, paths[i], cmd);
		if (!path)
			return (cleanup_and_return(pool, paths, NULL, NULL));
		if (access(path, X_OK) == 0)
			return (cleanup_and_return(pool, paths, path, path));
		if (!candidate && access(path, F_OK) == 0)
			candidate = path;
		else
			pool_free_ctx(pool, path);
		i++;
	}
	if (candidate)
		return (cleanup_and_return(pool, paths, candidate, candidate));
	return (cleanup_and_return(pool, paths, NULL, NULL));
}

// Resolves the command path using PATH or returns NULL when not found.
char	*find_command_path(t_expander *exp, const char *cmd)
{
	char	**paths;
	char	*path_var;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (pool_strdup_ctx(exp->pool, cmd));
		return (NULL);
	}
	path_var = get_env(exp->my_env, "PATH");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	if (!track_split(exp->pool, paths))
		return (NULL);
	return (find_command_path_2(exp->pool, cmd, paths));
}
