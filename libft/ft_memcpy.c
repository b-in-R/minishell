/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 12:28:16 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/06 19:27:42 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!dst && !src)
		return (NULL);
	while (i < n)
	{
		((char *)dst)[i] = ((char *)src)[i];
		i++;
	}
	return ((char *)dst);
}
/*
int	main(int ac, char **av)
{
	char	src[] = "123456789";
	char	dst[] = "qwertyu";
	size_t	n = 11;
	(void)av;

	if (ac == 1)
	{
		printf("test: %s\n", ft_memcpy(dst, NULL, n));
	}
	else
	{
		printf("vrai: %s\n", memcpy(dst, NULL, n));
	}
	return 0;
}
*/
