/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 17:49:15 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/28 17:49:18 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*res;
	int		i;

	i = 0;
	res = ft_calloc(ft_strlen(s1) + 1, sizeof(const char));
	if (!res)
		return (NULL);
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	return (res);
}
/*
int	main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("1 string pls\n");
		return 0;
	}
	printf("base: %s\ncopy: %s\n", av[1], ft_strdup(av[1]));
	printf("check base: %s\n", av[1]);// mettre +1 ligne 26
	return 0;
}
*/
