/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:08:33 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/06 19:28:44 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	ret;
	int		i;

	ret = 0;
	i = 0;
	while (src[ret])
		ret++;
	if (dstsize == 0)
		return (ret);
	while (dstsize > 1 && src[i])
	{
		dst[i] = src[i];
		i++;
		dstsize--;
	}
	dst[i] = '\0';
	return (ret);
}
/*
int	main(int ac, char **av)
{
	char	dst_1[] = "123456";
	const char	src_1[] = "qwertyuiop";
	size_t	dstsize_1 = 6;
	char	dst_2[] = "123456";
	const char	src_2[] = "qwertyuiop";
	size_t	dstsize_2 = 6;
	printf("1: vrai: %zu, %s\n   test: %zu, %s\n", 
			strlcpy(dst_1, src_1, dstsize_1), dst_1,
			ft_strlcpy(dst_2, src_2, dstsize_2), dst_2);

	char	dst_3[] = "1234567890";
	const char	src_3[] = "qwert";
	size_t	dstsize_3 = 2;
	char	dst_4[] = "1234567890";
	const char	src_4[] = "qwert";
	size_t	dstsize_4 = 2;
	printf("2: vrai: %zu, %s\n   test: %zu, %s\n",
			strlcpy(dst_3, src_3, dstsize_3), dst_3,
			ft_strlcpy(dst_4, src_4, dstsize_4), dst_4);
	
	char	dst_5[] = "1234567890";
	const char	src_5[] = "qwertyuiop";
	size_t	dstsize_5 = 6;
	char	dst_6[] = "1234567890";
	const char	src_6[] = "qwertyuiop";
	size_t	dstsize_6 = 6;
	printf("3: vrai: %zu, %s\n   test: %zu, %s\n",
			strlcpy(dst_5, src_5, dstsize_5), dst_5,
			ft_strlcpy(dst_6, src_6, dstsize_6), dst_6);
	
	char	dst_7[] = "1234567890";
	const char	src_7[] = "qwerty";
	size_t	dstsize_7 = sizeof(dst_7);
	char	dst_8[] = "1234567890";
	const char	src_8[] = "qwerty";
	size_t	dstsize_8 = sizeof(dst_8);
	printf("dstsize: %zu\n", dstsize_8);
	printf("4: vrai: %zu, %s\n   test: %zu, %s\n",
			strlcpy(dst_7, src_7, dstsize_7), dst_7,
			ft_strlcpy(dst_8, src_8, dstsize_8), dst_8);

	return 0;
}
*/
