/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:02:48 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/28 16:02:49 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((const unsigned char *)s)[i] == (unsigned char)c)
			return ((void *)((char *)s + i));
		i++;
	}
	return (NULL);
}
/*
int	main(void)
{
	const char	s[] = "1234567890";
	char		c = '5';
	size_t		n = 4;

	printf("vrai: %s\n", (char *)memchr(s, c, n));
	printf("test: %s\n", (char *)ft_memchr(s, c, n));
	return 0;
}
*/
