/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:04:46 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/28 16:04:48 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while ((s1[i] || s2[i]) && s1[i] == s2[i] && n > 1)
	{
		i++;
		n--;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
/*
int	main(void)
{
	const char	s1[] = "abcdef";
	const char	s2[] = "abc\375xx";
	size_t		n = 5;

	printf("vrai: %i\n", strncmp(s1, s2, n));
	printf("test: %i\n", ft_strncmp(s1, s2, n));
	return 0;
}
*/
