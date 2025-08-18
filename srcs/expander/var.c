/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:47:55 by albertooutu       #+#    #+#             */
/*   Updated: 2025/08/15 19:54:27 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
*	Checks if the line is a simple assignment of the form "KEY=VALUE".
*/

/*
//
// modif test rabiner pour problemes env
//

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

// Updates the value of an environment variable my_env
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

*/


/*
** Détecte si la ligne est une assignation simple: NAME=VALUE, sans commande autour.
** - NAME doit être un identifiant valide (même règle que export/unset)
** - Il doit y avoir un '='
** - On n'essaie pas ici de refuser des espaces autour (ton lexer s’en occupe).
*/
int	is_simple_assignment(const char *line)
{
	size_t i = 0;

	if (!line || !*line)
		return 0;
	while (line[i] && line[i] != '=')
		i++;
	if (line[i] != '=')
		return 0;
	// Valider l'identifiant avant '='
	return is_valid_identifier(line);                // CHANGED: réutilise ta validation d’identifiant
}

/*
** Ajoute/maj dans local_env UNIQUEMENT (pas d'export).
** *env peut être NULL -> set_env gère la création du tableau.
** Retour: 0 ok, 1 erreur (alloc ou identifiant invalide).
*/
int	add_env_variable(char ***env, const char *line)
{
	char **newtab;

	if (!env || !line)
		return 1;                                    // CHANGED: garde invariant simple
	if (!is_valid_identifier(line) || !ft_strchr(line, '='))
		return 1;                                    // CHANGED: refuse les formes invalides

	newtab = set_env(*env, line);                   // CHANGED: set_env peut réallouer
	if (!newtab)
		return 1;                                    // CHANGED: échec alloc (rare)

	*env = newtab;                                   // CHANGED: propager la réalloc éventuelle à l’appelant
	return 0;


/*
* Used to handle all normal characters that aren't $
* Adds a character to the end of a string dynamically.
* Puts the character in the buffer
* Concatenates the buffer to the existing string and frees the old string
*/
  /*
int	append_char(char **str, char c)
{
	char	buff[2];

	buff[0] = c;
	buff[1] = '\0';
	return (str_append_free(str, buff));
}
*/
/*
* Dynamically concatenates two strings.
* Performs a classic strjoin, but frees s1 to avoid memory leaks.
* Used in append_char() to add a character to the result string.
* and also in handle_dollar() to add the value of a variable to result.
*/
  /*
int	str_append_free(char **s1, const char *s2)
{
	char	*tmp;

	if (!s1 || !s2)
		return (0);
	if (!*s1)
		tmp = strdup(s2);
	else
	{
		tmp = malloc(strlen(*s1) + strlen(s2) + 1);
		if (!tmp)
			return (0);
		strcpy(tmp, *s1);
		strcat(tmp, s2);
	}
	if (!tmp)
		return (0);
	free(*s1);
	*s1 = tmp;
	return (1);

}
*/
