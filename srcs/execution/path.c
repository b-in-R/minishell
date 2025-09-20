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

// Builds a candidate executable path from directory + command name.
char	*join_path(const char *dir, const char *cmd)
{
	char	*path;
	char	*tmp;

	tmp = pool_strjoin_ctx(dir, "/");
	if (!tmp)
		return (NULL);
	path = pool_strjoin_ctx(tmp, cmd);
	if (!path)
	{
		pool_free_ctx(tmp);
		return (NULL);
	}
	pool_free_ctx(tmp);
	return (path);
}

// Adds every element of a split PATH array to the pool tracking.
static void	track_split(char **tab)
{
	int	i;

	if (!tab)
		return ;
	pool_track_ctx(tab);
	i = 0;
	while (tab[i])
	{
		pool_track_ctx(tab[i]);
		i++;
	}
}

// Frees a previously tracked split array using the pool helpers.
static void	free_split(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		pool_free_ctx(tab[i]);
		i++;
	}
	pool_free_ctx(tab);
}

// Iterates PATH entries to locate an executable with X_OK permission.
char	*find_command_path_2(const char *cmd, char **paths)
{
	int		i;
	char	*path;

	i = 0;
	while (paths[i])
	{
		path = join_path(paths[i], cmd);
		if (path && access(path, X_OK) == 0)
		{
			free_split(paths);
			return (path);
		}
		pool_free_ctx(path);
		i++;
	}
	free_split(paths);
	return (NULL);
}

// Resolves the command path using PATH or returns NULL when not found.
char	*find_command_path(char **my_env, const char *cmd)
{
	char	**paths;
	char	*path_var;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (pool_strdup_ctx(cmd));
		return (NULL);
	}
	path_var = get_env(my_env, "PATH");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	track_split(paths);
	return (find_command_path_2(cmd, paths));
}
