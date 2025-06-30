/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:43:15 by albertooutu       #+#    #+#             */
/*   Updated: 2025/06/24 12:55:43 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
*	Vérifie si le caractère c est un caractère valide pour une variable d'environnement.
*	Un caractère valide est alphanumérique (a-z, A-Z, 0-9) ou un underscore (_).
*/
int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/*
*	gére l'expansion d’un $ dans une chaîne ($USER, $?, $VAR123, etc.) et ajoute le résultat à la chaîne result
*	 - str : chaîne complète qu’on est en train d’analyser (ex. "echo $USER").
*	 - i : pointeur sur l’indice actuel (pour avancer dans word).
*	 - result : pointeur sur la chaîne qu’on est en train de construire (le mot après expansion).
*	 - last_status : valeur de $? (code de retour de la dernière commande).
*
*	Si on trouve un $ suivi de ?, on ajoute la valeur de last_status à result.
*	Si on trouve un $ suivi d’un nom de variable, start = *i pour marquer le début du nom de variable
*	et on increment i tant que alphanumérique ou underscore,et on extrait le nom de la variable, on récupère sa valeur avec getenv() et on l’ajoute à result.
*/
int	handle_dollar(const char *str, int *i, char **result, int last_status)
{
	char	*key;
	char	*value;
	int		start;

	(*i)++; // skip le $
	if (str[*i] == '?')
	{
		value = ft_itoa(last_status);
		if (!value)
			return (0);
		if (!str_append_free(result, value))
			return (free(value), 0);
		(*i)++;
		return (1);
	}
	start = *i;
	while (is_var_char(str[*i]))
		(*i)++;
	key = ft_substr(str, start, *i - start);
	if (!key)
		return (0);
	value = get_env_value(key);
	free(key);
	if (!value)
		return (0);
	if (!str_append_free(result, value))
		return (free(value), 0);
	free(value);
	return (1);
}

/*
*	Utilisé pour gérer tous les caractères normaux qui ne sont pas $
*	rajoute un caractère à la fin d’une chaîne dynamiquement.
*/
int	append_char(char **str, char c)
{
	char	buff[2];
	buff[0] = c; // Met le caractère dans le buffer
	buff[1] = '\0';
	return (str_append_free(str, buff)); // Concatène le buffer à la chaîne existante et libère l'ancienne chaîne
}

/*
*	Concatène deux chaînes de caractères dynamiquement.
*	Fait un strjoin classique mais en libérant s1 pour éviter les fuites mémoire.
*	Utilsé dans append_char() pour ajouter un caractère à la chaîne result.
*	et aussi dans handle_dollar() pour ajouter la valeur d’une variable à result.
*/
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

// obtenir la valuer de l'environnement ($USER, $PATH) avec getenv()
char	*get_env_value(const char *key)
{
	char	*value;

	value = getenv(key);
	if (!value)
		return (ft_strdup("")); // Si la variable n'existe pas, retourne une chaîne vide
	return (ft_strdup(value)); // Duplique la valeur trouvée
}
