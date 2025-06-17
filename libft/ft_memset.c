/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:34:12 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/14 18:14:21 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;
	char	*res;

	res = (char *) b;
	i = 0;
	while (i < len)
	{
		res[i] = c;
		i++;
	}
	return (b);
}
/*
int	main(void)
{
	char			v = 'A';
	char			t = 'z';

	int				buf = 10;
	char			buffer[] = "0123456789";
	char			*v_result;
	char			*t_result;
	unsigned long	i = 0;

	printf("\ntest:\n");
	t_result = ft_memset(buffer, t, buf);
	if (t_result == buffer)
		printf("ok %p\n", (void *)t_result);
	else
		printf("err adresse\n");
	printf("contenu: ");
	while (i < sizeof(buffer))
		printf("%c ", buffer[i++]);

	i = 0;
	printf("\n\nvrai:\n");
	v_result = memset(buffer, v, sizeof(buffer));
	if (v_result == buffer)
		printf("ok %p\n", (void *)v_result);
	else
		printf("err adresse\n");
	printf("contenu: ");
	while (i < sizeof(buffer))
	{
		printf("%c ", buffer[i++]);
	}

	printf("\n");
	return 0;
}
*/
