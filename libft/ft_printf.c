/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:31:05 by rabiner           #+#    #+#             */
/*   Updated: 2024/12/16 18:25:43 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	check_ptr(char format, va_list ap)
{
	void	*ptr;
	int		nb;

	nb = 0;
	ptr = va_arg(ap, void *);
	if (ptr == NULL)
		nb += write(1, NULL_PTR_STR, NULL_PTR_LEN);
	else
	{
		nb += write(1, "0x", 2);
		nb += ft_puthex((unsigned long)ptr, format);
	}
	return (nb);
}

int	check_uint(char format, va_list ap)
{
	unsigned int	nbmax;
	int				nb;

	nb = 0;
	nbmax = va_arg(ap, unsigned long);
	if (nbmax >= 4294967295)
		nb += ft_putstr("4294967295");
	else
		nb += ft_putnbr(nbmax, format);
	return (nb);
}

int	check_format(char format, va_list ap)
{
	int				nb;

	nb = 0;
	if (format == 'c')
		nb += ft_putchar(va_arg(ap, int));
	else if (format == 's')
		nb += ft_putstr(va_arg(ap, char *));
	else if (format == 'p')
		nb += check_ptr(format, ap);
	else if (format == 'd' || format == 'i')
		nb += ft_putnbr(va_arg(ap, int), format);
	else if (format == 'u')
		nb += check_uint(format, ap);
	else if (format == 'x' || format == 'X')
		nb += ft_puthex(va_arg(ap, unsigned int), format);
	else
		nb += write(1, &format, 1);
	return (nb);
}

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		nb;

	va_start(ap, format);
	nb = 0;
	while (*format)
	{
		if (*format == '%' && *(format + 1) == '%')
		{
			nb += ft_putchar('%');
			format++;
		}
		else if (*format == '%')
			nb += check_format(*(++format), ap);
		else
			nb += ft_putchar(*format);
		format++;
	}
	va_end(ap);
	return (nb);
}
