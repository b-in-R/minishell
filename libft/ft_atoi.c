/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:48:27 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/13 18:29:32 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	n;
	int	neg;

	i = 0;
	n = 0;
	neg = 1;
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			neg *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		n = (n * 10) + (str[i] - 48);
		i++;
	}
	return (n * neg);
}
/*
int main(void)
{
    const char str1[] = "  -129sdf";
    printf("1\nv: %i\nt: %i\n\n", atoi(str1), ft_atoi(str1));
      const char str2[] = "  -0123456sdf";
    printf("2\nv: %i\nt: %i\n\n", atoi(str2), ft_atoi(str2));
      const char str3[] = "  -sdf";
    printf("3\nv: %i\nt: %i\n\n", atoi(str3), ft_atoi(str3));
      const char str4[] = "  -0sdf";
    printf("4\nv: %i\nt: %i\n\n", atoi(str4), ft_atoi(str4));
      const char str5[] = "  +sdf";
    printf("5\nv: %i\nt: %i\n\n", atoi(str5), ft_atoi(str5));
      const char str6[] = "  2147483647sdf";
    printf("6\nv: %i\nt: %i\n\n", atoi(str6), ft_atoi(str6));
      const char str7[] = "  +2147483647sdf";
    printf("7\nv: %i\nt: %i\n\n", atoi(str7), ft_atoi(str7));
      const char str8[] = "  -2147483648sdf";
    printf("8\nv: %i\nt: %i\n\n", atoi(str8), ft_atoi(str8));
      const char str9[] = "  2147483648sd";
    printf("9\nv: %i\nt: %i\n\n", atoi(str9), ft_atoi(str9));
    return 0;
}
*/