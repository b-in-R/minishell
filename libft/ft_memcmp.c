/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:11:08 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/28 16:11:10 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n <= 0)
		return (0);
	while (i < n - 1 && ((unsigned char *)s1)[i] == ((unsigned char *)s2)[i])
	{
		i++;
	}
	return ((int)((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
}
/*
int	main(int ac, char **av)
{
	if (ac != 4)
	{
		printf("2 string + n\n");
		return 0;
	}
	printf("1: %s | 2: %s | n: %i\n", av[1], av[2], av[3][0] - 48);
	printf("vrai: %i\n", memcmp((const char *)av[1],
							 (const char *)av[2], av[3][0] - 48));
	printf("test: %i\n", ft_memcmp((const char *)av[1],
								(const char *)av[2], av[3][0] - 48));
	return 0;
}
*/
