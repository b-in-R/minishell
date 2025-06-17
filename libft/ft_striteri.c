/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:26:39 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/28 15:26:51 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
#include "libft.h"

static void	ft_toupper_iteri(unsigned int i, char *c)
{
	(void)i;
	*c = ft_toupper(*c);
}
*/
#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}
/*
int	main(void)
{
	char	s[] = "chevre en majuscule svp";
	printf("avant: %s\n", s);
	ft_striteri(s, ft_toupper_iteri);
	printf("apres: %s\n", s);
	return 0;
}
*/
