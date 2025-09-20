/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:43:15 by albertooutu       #+#    #+#             */
/*   Updated: 2025/09/12 00:30:58 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
* Checks if the character c is a valid character for an environment variable.
* A valid character is alphanumeric (a-z, A-Z, 0-9) or an underscore (_).
*/
int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/*
* Gère l'expansion de $? (exit status)
*/
static int	expand_exit_status(char **result, t_expander *exp, int *i)
{
	char	*value;

	value = ft_itoa(exp->last_status);
	if (!value)
		return (0);
	pool_track_ctx(value);
	if (!str_append_free(result, value))
		return (pool_free_ctx(value), 0);
	pool_free_ctx(value);
	(*i)++;
	return (1);
}

/*
* Extrait le nom de variable après $
*/
int	exp_variable(const char *str, int *i, char **result, t_expander *exp)
{
	char	*key;
	char	*value;
	int		start;

	start = *i;
	while (is_var_char(str[*i]))
		(*i)++;
	key = pool_substr_ctx(str, start, *i - start);
	if (!key)
		return (0);
	value = get_env_value_from_exp(key, exp);
	pool_free_ctx(key);
	if (!value)
		return (0);
	if (!str_append_free(result, value))
		return (pool_free_ctx(value), 0);
	pool_free_ctx(value);
	return (1);
}

/*
* Handles the expansion of a $ character in a string
* ($USER, $?, $VAR123, etc.) and appends the result to the string result.
* 	- str: The entire string being parsed (e.g., "echo $USER").
* 	- i: Pointer to the current index (to move forward in word).
* 	- result: Pointer to the string being constructed(the word after expansion)
* 	- last_status: The value of $? (return code of the last command).
*
* If a $ character is found followed by a ? character,
* 	add the value of last_status to result.
* If a $ character is found followed by a variable name,
*	start = *i to mark the beginning of the variable name.
* Increment i as alphanumeric or underscore, extract the variable name,
*	retrieve its value with getenv(), and append it to result.
*/
int	handle_dollar(const char *str, int *i, char **result, t_expander *exp)
{
	(*i)++;
	if (str[*i] == '?')
		return (expand_exit_status(result, exp, i));
	else
		return (exp_variable(str, i, result, exp));
}

/*
*	Retrieves the value of an environment variable from the expander
* 	first checks the local environment, then the global environment (export)
*/
char	*get_env_value_from_exp(const char *key, t_expander *exp)
{
	char	*val;

	val = get_env(exp->local_env, (char *)key);
	if (val)
		return (pool_strdup_ctx(val));
	val = get_env(exp->my_env, (char *)key);
	if (val)
		return (pool_strdup_ctx(val));
	return (pool_strdup_ctx(""));
}
