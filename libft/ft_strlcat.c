/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 18:05:08 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/09 17:52:46 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
   cc ft_ -lbsd
*/

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	dst_len;
	size_t	src_len;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (size <= dst_len)
		return (size + src_len);
	i = dst_len;
	j = 0;
	while (src[j] && (i < size - 1))
	{
		dst[i++] = src[j++];
	}
	dst[i] = '\0';
	return (dst_len + src_len);
}
/*
int	main(void)
{
	char	dst_7[20] = "1234567890";
	const char	src_7[] = "qwert";
	size_t	size_7 = 19;
	char	dst_8[20] = "1234567890";
	const char	src_8[] = "qwert";
	size_t	size_8 = 19;
	printf("4: vrai: %zu, %s\n   test: %zu, %s\n",
			strlcat(dst_7, src_7, size_7), dst_7,
			ft_strlcat(dst_8, src_8, size_8), dst_8);

	char	dst_1[20] = "1234567890";
	const char	src_1[] = "qwerty";
	size_t	size_1 = 14;
	char	dst_2[20] = "1234567890";
	const char	src_2[] = "qwerty";
	size_t	size_2 = 14;
	printf("1: vrai: %zu, %s\n   test: %zu, %s\n", 
			strlcat(dst_1, src_1, size_1), dst_1,
			ft_strlcat(dst_2, src_2, size_2), dst_2);

	char	dst_3[20] = "1234567890";
	const char	src_3[] = "qwerty";
	size_t	size_3 = 5;
	char	dst_4[20] = "1234567890";
	const char	src_4[] = "qwerty";
	size_t	size_4 = 5;
	printf("2: vrai: %zu, %s\n   test: %zu, %s\n",
			strlcat(dst_3, src_3, size_3), dst_3,
			ft_strlcat(dst_4, src_4, size_4), dst_4);
	
	char	dst_5[20] = "123456";
	const char	src_5[] = "qwertyuiop";
	size_t	size_5 = 0;
	char	dst_6[20] = "123456";
	const char	src_6[] = "qwertyuiop";
	size_t	size_6 = 0;
	printf("3: vrai: %zu, %s\n   test: %zu, %s\n",
			strlcat(dst_5, src_5, size_5), dst_5,
			ft_strlcat(dst_6, src_6, size_6), dst_6);

	return 0;
}
*/
