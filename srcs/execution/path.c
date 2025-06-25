/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:02:57 by rabiner           #+#    #+#             */
/*   Updated: 2025/06/22 19:53:17 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*join_path(const char *dir, const char *cmd)
{
	char	*path;
	char	*tmp;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (path);
}

char	*find_command_path(const char *cmd)
{
	char	**paths;
	char	*path;
	char	*path_var;
	int		i;

	// si la commande contient un '/' => chemin direct
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}

	// sinon, on cherche dans $PATH
	
	// test
	//printf(YELL"find_cmd-path 1\n"RST);
	//path_var = get_env("PATH");
	path_var = getenv("PATH");
	//printf(YELL"path_var:\t%s\n", path_var);

	
	if (!path_var)
		return (NULL);

	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);

		/*
	// test: voir les path
	int	j = 0;
	while (paths[j])
	{
		printf(YELL"paths[%i]:\t%s\n"RST, j, paths[j]);
		j++;
	}
	j = 0;
	*/

	i = 0;
	while (paths[i])
	{
		path = join_path(paths[i], cmd);
		// test
		//printf(YELL"path[%i]:\t%s\n"RST, i, path);
		
		if (path && access(path, X_OK) == 0)
		{
			ft_split_free(paths);
			// test
			printf(CYAN"path access ok:\t%s\n"RST, path);
			
			return (path);
		}
		free(path);
		i++;
	}
	ft_split_free(paths);
	return (NULL);
}
