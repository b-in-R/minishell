/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:47:55 by albertooutu       #+#    #+#             */
/*   Updated: 2025/08/13 15:06:21 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
*	Checks if the line is a simple assignment of the form "KEY=VALUE".
*/
int	is_simple_assignment(const char *line)
{
	int	i;

	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (!line[i] || !(ft_isalpha(line[i]) || line[i] == '_'))
		return (0);
	while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
		i++;
	if (line[i] != '=')
		return (0);
	i++;
	while (line[i])
	{
		if (line[i] == '|' || line[i] == '<' || line[i] == '>')
			return (0);
		i++;
	}
	return (1);
}

/*
*	Updates the value of an environment variable my_env
*/
int	add_env_variable(char ***env, const char *line)
{
	char	*trimmed_line;
	char	**result;
	int		i;
	int		j;

	if (!line || !env)
		return (0);
	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	trimmed_line = ft_strdup(line + i);
	if (!trimmed_line)
		return (0);
	j = ft_strlen(trimmed_line) - 1;
	while (j >= 0 && (trimmed_line[j] == ' ' || trimmed_line[j] == '\t'))
		trimmed_line[j--] = '\0';
	result = set_env(*env, trimmed_line);
	free(trimmed_line);
	if (!result)
		return (0);
	*env = result;
	return (1);
}
