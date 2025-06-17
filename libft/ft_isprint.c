/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:19:36 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/02 16:16:08 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if (c > 31 && c < 127)
		return (1);
	else
		return (0);
}
/*
int	main(int ac, char **av)
{
	if (ac != 2)
		return 0;
	printf("%c: %i\n", av[1][0], ft_isprint(av[1][0]));
	return 0;
}
*/
