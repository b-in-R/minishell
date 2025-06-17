/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:07:58 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/15 17:08:00 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	len;

	if (!s1)
		return (NULL);
	if (!set || set[0] == '\0')
		return (ft_substr(s1, 0, ft_strlen(s1)));
	i = 0;
	len = ft_strlen(s1);
	while (s1[i] && ft_strchr(set, s1[i]) != NULL)
		i++;
	while (len > i && ft_strchr(set, s1[len - 1]) != NULL)
		len--;
	return (ft_substr(s1, i, len - i));
}
/*
int	main(void)
{
	const char	s1[] = "tresalutywqer. aqyw";
	const char	set[] = "qwerty";
	char		*res;

	res = ft_strtrim(s1, set);
	printf("%s\n", res);
	free(res);
	return 0;
}
*/
