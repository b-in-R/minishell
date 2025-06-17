/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:37:38 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/28 15:37:40 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	res;
	int		i;

	res = (char) c;
	i = ft_strlen(s);
	if (res == '\0')
		return ((char *)&s[i]);
	while (i >= 0)
	{
		if ((char)s[i] == res)
			return ((char *)&s[i]);
		i--;
	}
	return (NULL);
}
/*
int	main(int argc, char **argv)
{

	(void)argc;
	(void)argv;
	const char	*res_t;
	const char	*res_v;
	res_t = ft_strrchr("bonjour", '\0');
	res_v = strrchr("bonjour", '\0');
	printf("test: %s\n", res_t);
	printf("vrai: %s\n", res_v);
	return 0;
}
	*/
