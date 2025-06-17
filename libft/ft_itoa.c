/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:22:22 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/28 15:22:36 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*createtab(int i)
{
	char	*tab;

	tab = malloc((i + 1) * sizeof(char));
	if (!tab)
		return (NULL);
	tab[0] = '0';
	return (tab);
}

long	charcounter(int nbr)
{
	int	i;

	i = 0 ;
	if (nbr < 0)
	{
		i ++;
		nbr = -nbr;
	}
	if (nbr == 0)
		i ++;
	while (nbr != 0)
	{
		nbr /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	int		len;
	int		i;
	char	*res;
	long	nbr;

	nbr = n;
	len = charcounter(nbr);
	res = createtab(len);
	if (!res)
		return (NULL);
	if (nbr < 0)
	{
		nbr = -nbr;
		res[0] = '-';
	}
	i = len -1;
	while (nbr != 0)
	{
		res[i] = ((nbr % 10) + 48);
		nbr = nbr / 10;
		i--;
	}
	res[len] = '\0';
	return (res);
}
/*
int	main(void)
{
	int	n = -2354;
	printf("%s\n", ft_itoa(n));
	return 0;
}
*/
