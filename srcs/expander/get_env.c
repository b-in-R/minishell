/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:43:15 by albertooutu       #+#    #+#             */
/*   Updated: 2025/09/27 20:45:43 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
* Extrait le nom de variable après $
*/
int	exp_variable(t_expand *expand, const char *str, int *i)
{
	char	*key;
	char	*value;
	int		start;

	start = *i;
	while (is_var_char(str[*i]))
		(*i)++;
	key = pool_substr(expand->pool, str, start, *i - start);
	if (!key)
		return (0);
	value = get_env_value_from_exp(key, expand->exp);
	pool_free_one(expand->pool, key);
	if (!value)
		return (0);
	if (!str_append_free(expand->pool, expand->result, value))
		return (pool_free_one(expand->exp->pool, value), 0);
	pool_free_one(expand->exp->pool, value);
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
int	handle_dollar(t_expand *expand, const char *str, int *i)
{
	char	next;

	(*i)++;
	next = str[*i];
	if (!next)
		return (append_char(expand->pool, expand->result, '$'));
	if (next == '?')
		return (expand_exit_status(expand->pool, expand->result,
				expand->exp, i));
	if (next == '$')
		return (expand_pid(expand->pool, expand->result, i));
	if (!is_var_char(next))
	{
		if (!append_char(expand->pool, expand->result, '$'))
			return (0);
		return (1);
	}
	if (!exp_variable(expand, str, i))
		return (0);
	return (1);
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
		return (pool_strdup(exp->pool, val));
	val = get_env(exp->my_env, (char *)key);
	if (val)
		return (pool_strdup(exp->pool, val));
	return (pool_strdup(exp->pool, ""));
}
