/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:19:42 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/14 18:29:58 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*res;
	size_t	i;
	size_t	l;

	i = 0;
	l = ft_strlen(s);
	if (start >= l)
		return (ft_calloc(1, sizeof(char)));
	if (len > l - start)
		len = l - start;
	res = ft_calloc(len + 1, sizeof(char));
	if (!res)
		return (NULL);
	while (i < len && s[i])
	{
		res[i] = s[start];
		i++;
		start++;
	}
	return (res);
}
/*
int	main(void)
{
	const char		s[] = "hola";
	unsigned int	start = 0;
	size_t			len = -1;
	char			*res;

	res = ft_substr(s, start, len);
	printf("%s\n", res);
	free(res);
	return 0;
}
*/
