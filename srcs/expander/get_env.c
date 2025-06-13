/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:43:15 by albertooutu       #+#    #+#             */
/*   Updated: 2025/06/13 15:28:43 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
*	sert à suivre si on est à l’intérieur de quotes simples (') ou doubles ("), pendant qu’on parcourt une chaîne de caractères (un token WORD).
*	Si on rencontre un simple quote ('), (et qu’on n’est pas dans des "")  on inverse in_single.
* 	Si on rencontre un double quote ("), (et qu’on n’est pas dans des '') on inverse in_double.
*/
void	update_quote_flags(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !*in_double)
		*in_single = !*in_single;
	else if (c == '"' && !*in_single)
		*in_double = !*in_double;
}

/*
*	gére l'expansion d’un $ dans une chaîne ($USER, $?, $VAR123, etc.) et ajoute le résultat à la chaîne result
*	 - word : chaîne complète qu’on est en train d’analyser (ex. "echo $USER").
*	 - i : pointeur sur l’indice actuel (pour avancer dans word).
*	 - result : pointeur sur la chaîne qu’on est en train de construire (le mot après expansion).
*	 - last_status : valeur de $? (code de retour de la dernière commande).
*
*	Si on trouve un $ suivi de ?, on ajoute la valeur de last_status à result.
*	Si on trouve un $ suivi d’un nom de variable, start = *i pour marquer le début du nom de variable
*	et on increment i tant que alphanumérique ou underscore,et on extrait le nom de la variable, on récupère sa valeur avec getenv() et on l’ajoute à result.
*/
void	handle_dollar(const char *word, int *i, char **result, int last_status)
{
	char	*var;
	char	*value;
	int		start;

	(*i)++;
	if (word[*i] == '?')
	{
		value = ft_itoa(last_status);
		*result = str_append_free(*result, value);
		free(value);
		(*i)++;
	}
	else
	{
		start = *i;
		while (word[*i] && (ft_isalnum(word[*i]) || word[*i] == '_'))
			(*i)++;
		var = ft_substr(word, start, *i - start); // Extrait le nom de la variable
		value = get_env_value(var); // Récupère la valeur de la variable d’environnement
		*result = str_append_free(*result, value); // Ajoute la valeur à result
		free(var);
		free(value);
	}
}

/*
*	Utilisé pour gérer tous les caractères normaux qui ne sont pas $
*	rajoute un caractère à la fin d’une chaîne dynamiquement.
*/
void	append_char(char **str, char c)
{
	char	tmp[2];
	char	*to_add;

	tmp[0] = c;
	tmp[1] = '\0';
	to_add = ft_strdup(tmp); // Duplique le caractère en chaîne de caractères
	*str = str_append_free(*str, to_add); // Concatène et libère l'ancienne chaîne
	free(to_add);
}

/*
*	Concatène deux chaînes de caractères dynamiquement.
*	Fait un strjoin classique mais en libérant s1 pour éviter les fuites mémoire.
*	Utilsé dans append_char() pour ajouter un caractère à la chaîne result.
*	et aussi dans handle_dollar() pour ajouter la valeur d’une variable à result.
*/
char	*str_append_free(char *s1, const char *s2)
{
	char	*joined;

	if (!s1)
		return (ft_strdup(s2));
	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}

// obtenir la valuer de l'environnement ($USER, $PATH) avec getenv()
char	*get_env_value(const char *word)
{
	char	*value;

	value = getenv(word);
	if (value)
		return (ft_strdup(value)); // Duplique la valeur trouvée
	else
		return (ft_strdup("")); // Si pas trouvé, retourne une chaîne vide
}
