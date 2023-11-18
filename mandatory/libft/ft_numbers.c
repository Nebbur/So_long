/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_numbers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42porto.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:19:58 by rboia-pe          #+#    #+#             */
/*   Updated: 2023/04/07 02:19:58 by rboia-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	hexa(unsigned long nbr, char *base, unsigned int size, t_flags *f)
{
	if (nbr >= size)
	{
		hexa(nbr / size, base, size, f);
		hexa(nbr % size, base, size, f);
	}
	else
		f->len += ft_putchar(base[nbr]);
}

void	type_s(t_flags *f, char *str)
{
	if (str == NULL)
		str = "(null)";
	while (*str)
		f->len += ft_putchar(*str++);
}

void	type_i(t_flags	*f)
{
	int			digits[10];
	int			digit_count;

	f->n = va_arg(f->args, int);
	digit_count = 0;
	if (f->n < 0)
	{
		f->n *= -1;
		f->len += ft_putchar('-');
	}
	while (f->n > 0 || digit_count == 0)
	{
		digits[digit_count++] = f->n % 10;
		f->n /= 10;
	}
	while (--digit_count >= 0)
		f->len += ft_putchar ('0' + digits[digit_count]);
}

void	ft_diux(t_flags *f)
{
	if (f->specifier_type == 'i' || f->specifier_type == 'd')
		type_i(f);
	else if (f->specifier_type == 'u')
		hexa(va_arg(f->args, unsigned int), "0123456789", 10, f);
	else if (f->specifier_type == 'x')
		hexa(va_arg(f->args, unsigned int), "0123456789abcdef", 16, f);
	else if (f->specifier_type == 'X')
		hexa(va_arg(f->args, unsigned int), "0123456789ABCDEF", 16, f);
	else if (f->specifier_type == 'c')
		f->len += ft_putchar((char)va_arg(f->args, int));
	else if (f->specifier_type == 's')
		type_s(f, va_arg(f->args, char *));
	else
	{
		f->n = va_arg(f->args, long);
		if (f->n == 0)
			f->len += (ft_putstr("(nil)"));
		else
		{
			f->len += ft_putstr ("0x");
			hexa(f->n, "0123456789abcdef", 16, f);
		}
	}
}
