/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:07:43 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/02 16:13:00 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, '\0', n);
}
/*
int	main(void)
{
	char	str[] = "0123456789";
	size_t	n_v = 7;
	size_t	n_t = 4;
	int		i = 0;

	printf("start: %s\n\n", str);
	ft_bzero(str, n_t);
	printf("t:\n");
	while (i < 10)
	{
		printf("%c, ", str[i]);
		i++;
	}
	i = 0;
	bzero(str, n_v);
	printf("\n\nv:\n");
	while (i < 10)
	{
		printf("%c, ", str[i]);
		i++;
	}
	return 0;
}
*/
