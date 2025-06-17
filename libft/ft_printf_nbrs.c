/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_nbrs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:31:05 by rabiner           #+#    #+#             */
/*   Updated: 2024/12/28 18:25:43 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_puthex(unsigned long nbr, char format)
{
	int	nb;

	if (nbr == 0)
		return (write(1, "0", 1));
	else if (nbr >= 16)
	{
		nb = ft_puthex(nbr / 16, format);
		return (nb + ft_puthex(nbr % 16, format));
	}
	else if (nbr <= 9)
		return (ft_putchar(nbr + 48));
	else if (format == 'X')
		return (ft_putchar(SYMB_MAX[nbr]));
	else
		return (ft_putchar(SYMB_MIN[nbr]));
}

int	ft_putnbr(long nbr, char format)
{
	int		nb;

	if (format == 'u' && nbr < 0)
		nbr = (unsigned long)nbr;
	if (nbr == -2147483648 && (format == 'i' || format == 'd'))
		return (nb = write(1, "-2147483648", 11));
	else if (nbr < 0 && (format == 'i' || format == 'd'))
	{
		nb = write(1, "-", 1);
		return (ft_putnbr(-nbr, format) + 1);
	}
	else if (nbr > 9)
	{
		nb = ft_putnbr(nbr / 10, format);
		return (nb + ft_putnbr(nbr % 10, format));
	}
	else
		return (ft_putchar(nbr + 48));
}
