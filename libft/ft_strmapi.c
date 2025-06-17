/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:27:00 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/28 15:27:03 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
#include "libft.h"

static	char	ft_toupper_mapi(unsigned int i, char c)
{
	(void)i;
	return (ft_toupper(c));
}
*/
#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*res;
	unsigned int	len;

	len = ft_strlen(s);
	res = malloc(((len + 1) * sizeof(char)));
	if (!res)
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = (*f)(i, s[i]);
		i++;
	}
	res[len] = 0;
	return (res);
}
/*
int	main(void)
{
	const char	s[] = "salut";
	char	*res = ft_strmapi(s, ft_toupper_mapi);
	printf("base: %s\nres: %s\n", s, res);
	free(res);
	return 0;
}
*/
