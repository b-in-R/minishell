/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 18:31:51 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/13 19:25:33 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*res;

	res = malloc(count * size);
	if (!res)
		return (NULL);
	res = ft_memset(res, 0, count * size);
	return (res);
}
/*
int	main(void)
{
//	int	*src1;
	int	*src2;
	size_t	count = 12;
	size_t	size = sizeof(int); 
	size_t	i = 0;
	printf("%lu\n", size);
//	src1 = calloc(count, size);
	src2 = ft_calloc(count, size);
//	if (!src2 || !src1)
//	{
//		printf("erreur allocation");
//		return 1;
//	}
//	printf("v:\n");
//	while (i < count)
//	{
//		printf("%d, ", src1[i]);
//		i++;
//	}
//	printf("\n");
//	i = 0;
//	printf("t:\n");
	while (i < count)
	{
		printf("%d, ", src2[i]);
		i++;
	}
	printf("\n");
//	free(src1);
	free(src2);
	return 0;
}
//cc -Wall -Wextra -Werror -fsanitize=address ft_calloc.c
*/
