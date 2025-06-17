/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 19:01:16 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/13 16:42:44 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	int	i;
	int	j;
	int	r;

	i = 0;
	r = 0;
	if (little[0] == '\0')
		return ((char *)&big[i]);
	while (big[i] && len > 0)
	{
		j = 0;
		while (big[i] == little[j] && len > 0)
		{
			if (little[j] == '\0' || (little[j + 1] == '\0'))
				return ((char *)&big[i - j]);
			i++;
			j++;
			len--;
		}
		r++;
		len = len + i - r;
		i = r;
	}
	return (NULL);
}
/*

voir pour mettre avec strlen et strncmp ou similaire


int	main(void)
{	
	//3
//	const char	big_3[] = ""; //21
//	const char	little_3[] = "";
//	size_t		len_3 = -1; //18 ok

//	printf("\n3:\nvrai:\n%s\n", strnstr(big_3, little_3, len_3));

	const char	big_4[] = "";
	const char	little_4[] = "";
	size_t		len_4 = -1;

	printf("test:\n%s\n.", ft_strnstr(big_4, little_4, len_4));

		//4
	//	const char	big_0[]= "lorem ipsum dolor sit amet"; //3
	//	const char	little_0[] = "lorem";
	//	size_t		len_0= -1; //21 ok
    //
//	//	printf("\n4:\nvrai:\n%s\n", strnstr(big_0, little_0, len_0));

	const char	big_0t[] = "lorem ipsum dolor sit amet"; //30
	const char	little_0t[] = "lorem";
	size_t		len_0t = -1;

	printf("test:\n%s\n", ft_strnstr(big_0t, little_0t, len_0t));

		//5
	//	const char	big_7[] = "001122230011223400112223456"; //28
	//	const char	little_7[] = "01122234";
	//	size_t		len_7 = 4; //26 ok
    //
//	//	printf("\n5:\nvrai:\n%s\n", strnstr(big_7, little_7, len_7));

	const char	big_8[] = "001122230011223400112223456";
	const char	little_8[] = "01122234";
	size_t		len_8 = 4;

	printf("test:\n%s\n", ft_strnstr(big_8, little_8, len_8));

		//1
	//	const char	big_5[] = "01 02 03 03 012 03 012 03 01FF"; //30  10  21
	//	const char	little_5[] = "03 012 03 01";
	//	size_t		len_5 = 18; //21 ok
    //
//	//	printf("\n1:\nvrai:\n%s\n", strnstr(big_5, little_5, len_5));

	const char	big_6[] = "01 02 03 03 012 03 012 03 01FF";
	const char	little_6[] = "03 012 03 01";
	size_t		len_6 = 18;

		printf("test:\n%s\n", ft_strnstr(big_6, little_6, len_6));
    //	
	//	//2
	//	const char	big_1[] = "01012012301234001234500123456"; //30
	// const char	little_1[] = "012345";
	//	size_t		len_1 = 22; //21 ok

//	printf("\n2:\nvrai:\n%s\n", strnstr(big_1, little_1, len_1));

	const char	big_2[] = "01012012301234001234500123456"; //30
	const char	little_2[] = "012345";
	size_t		len_2 = 22;

	printf("test:\n%s\n", ft_strnstr(big_2, little_2, len_2));


	return 0;
}
*/
