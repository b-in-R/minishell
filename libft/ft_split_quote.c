/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: binr <binr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:29:32 by binr              #+#    #+#             */
/*   Updated: 2025/05/02 23:21:43 by binr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_word_quote(const char *str, char sep)
{
	int	i;
	int	cnt;
	int	in_quote;

	i = 0;
	cnt = 0;
	in_quote = 0;
	while (str[i])
	{
		while (str[i] == sep && !in_quote)
			i++;
		if (str[i])
			cnt++;
		while (str[i] && (in_quote || str[i] != sep))
		{
			if (str[i] == '\'')
				in_quote = !in_quote;
			i++;
		}
	}
	return (cnt);
}

static char	*putword_quote(const char *str, int start, int end)
{
	char	*word;
	int		j;

	j = 0;
	word = malloc((end - start + 1) * sizeof(char));
	if (!word)
		return (NULL);
	while (start < end)
	{
		if (str[start] != '\'')
			word[j++] = str[start];
		start++;
	}
	word[j] = '\0';
	return (word);
}

static int	split_core(const char *s, char c, char **tab, t_split_quote *vars)
{
	while (vars->i <= ft_strlen(s))
	{
		if (s[vars->i] != c && vars->start_word < 0)
			vars->start_word = vars->i;
		else if ((s[vars->i] == c && !vars->in_quote)
			|| vars->i == ft_strlen(s))
		{
			if (vars->start_word >= 0)
			{
				tab[vars->j] = putword_quote(s, vars->start_word, vars->i);
				if (!tab[vars->j])
					return (1);
				vars->start_word = -1;
				vars->j++;
			}
		}
		if (s[vars->i] == '\'')
			vars->in_quote = !(vars->in_quote);
		vars->i++;
	}
	return (0);
}

char	**ft_split_quote(char const *s, char c)
{
	t_split_quote	vars;
	char			**tab;

	vars.i = 0;
	vars.j = 0;
	vars.start_word = -1;
	vars.in_quote = 0;
	tab = ft_calloc((count_word_quote(s, c) + 1), sizeof(char *));
	if (!tab)
		return (NULL);
	if (split_core(s, c, tab, &vars))
		return (NULL);
	return (tab);
}
