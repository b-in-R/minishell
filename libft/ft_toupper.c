/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 18:03:29 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/09 18:15:45 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c > 96 && c < 123)
		return (c - 32);
	else
		return (c);
}
/*
int	main(int ac, char **av)
{
	if (ac == 2 && (av[1][0] >= 48 && av[1][0] <= 122))
	{
		printf("%c\n", ft_toupper(av[1][0]));
	}
	else
		printf("arg a-z");
	return 0;
}
*/
