/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:43:15 by albertooutu       #+#    #+#             */
/*   Updated: 2025/09/26 12:33:37 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
* Extrait le nom de variable après $
*/
int	exp_variable(t_pool *pool, const char *str, int *i, char **result,
			t_expander *exp)
{
	char	*key;
	char	*value;
	int		start;

	start = *i;
	while (is_var_char(str[*i]))
		(*i)++;
	key = pool_substr_ctx(pool, str, start, *i - start);
	if (!key)
		return (0);
	value = get_env_value_from_exp(key, exp);
	pool_free_ctx(pool, key);
	if (!value)
		return (0);
	if (!str_append_free(pool, result, value))
		return (pool_free_ctx(exp->pool, value), 0);
	pool_free_ctx(exp->pool, value);
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
int	handle_dollar(t_pool *pool, const char *str, int *i, char **result,
			t_expander *exp)
{
	char	next;

	(*i)++;
	next = str[*i];
	if (!next)
		return (append_char(pool, result, '$'));
	if (next == '?')
		return (expand_exit_status(pool, result, exp, i));
	if (next == '$')
		return (expand_pid(pool, result, i));
	if (!is_var_char(next))
	{
		if (!append_char(pool, result, '$'))
			return (0);
		return (1);
	}
	if (!exp_variable(pool, str, i, result, exp))
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
		return (pool_strdup_ctx(exp->pool, val));
	val = get_env(exp->my_env, (char *)key);
	if (val)
		return (pool_strdup_ctx(exp->pool, val));
	return (pool_strdup_ctx(exp->pool, ""));
}
