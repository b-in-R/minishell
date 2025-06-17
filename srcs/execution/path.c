/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:02:57 by rabiner           #+#    #+#             */
/*   Updated: 2025/06/17 15:29:15 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

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
	path_var = get_env("PATH");
	if (!path_var)
		return (NULL);

	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);

	i = 0;
	while (paths[i])
	{
		path = join_path(paths[i], cmd);
		if (path && access(path, X_OK) == 0)
		{
			ft_split_free(paths);
			return (path);
		}
		free(path);
		i++;
	}
	ft_split_free(paths);
	return (NULL);
}
