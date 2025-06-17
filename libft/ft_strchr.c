/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 18:20:56 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/13 14:05:06 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char	*ft_strchr(const char *s, int c)
{
	unsigned char	res;
	int				i;

	res = (unsigned char) c;
	i = 0;
	while (s[i])
	{
		if ((unsigned char)s[i] == res)
			return ((char *)&s[i]);
		i++;
	}
	if (res == '\0')
		return ((char *)&s[i]);
	return (NULL);
}
/*
int	main(int ac, char **av)
{
	(void)ac;
	(void)av;
	const char *res;
	res = ft_strchr("tripouille", 'r' + 256);
	printf("%s\n", res);


	const char	*res;
	int	c = av[2][0];
	if (ac != 3)
		printf("arg string + c\n");
	else
	{
		if (c == 's')
			c = '\0';
		res = (ft_strchr(av[1], c));
		printf("%s\n", res);
	}

	return 0;

}


void	ft_print_result(char const *s)
{
	int		len;

	len = 0;
	while (s[len])
		len++;
	write(1, s, len);
}

int		main(int argc, const char *argv[])
{
	char		*str;
	char		str2[] = "bonjour";
	int			arg;

//	alarm(TIMEOUT);
	if (argc == 1)
		return (0);
	else if ((arg = atoi(argv[1])) == 1)
	{
		if (!(str = ft_strchr(str2, 'b')))
			ft_print_result("NULL");
		else
		{
			ft_print_result(str);
			if (str != str2)
				ft_print_result("\nReturn value is false");
		}
	}
	else if (arg == 2)
	{
		if (!(str = ft_strchr(str2, 'o')))
			ft_print_result("NULL");
		else
		{
			ft_print_result(str);
			if (str != str2 + 1)
				ft_print_result("\nReturn value is false");
		}
	}
	else if (arg == 3)
	{
		if (!(str = ft_strchr(str2, 'j')))
			ft_print_result("NULL");
		else
		{
			ft_print_result(str);
			if (str != str2 + 3)
				ft_print_result("\nReturn value is false");
		}
	}
	else if (arg == 4)
	{
		if (!(str = ft_strchr(str2, 's')))
			ft_print_result("NULL");
		else
			ft_print_result(str);
	}
	else if (arg == 5)
	{
		if (!(str = ft_strchr(str2, '\0')))
			ft_print_result("NULL");
		else
		{
			ft_print_result(str);
			if (str != str2 + 7)
				ft_print_result("\nReturn value is false");
		}
	}
	else if (arg == 6)
	{
		char	str3[] = "";
		if (!(str = ft_strchr(str3, '\0')))
			ft_print_result("NULL");
		else
		{
			ft_print_result(str);
			if (str != str3)
				ft_print_result("\nReturn value is false");
		}
	}
	return (0);
}
*/
