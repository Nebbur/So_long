/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42porto.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:19:58 by rboia-pe          #+#    #+#             */
/*   Updated: 2023/04/07 02:19:58 by rboia-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printf(const char *format, ...)
{
	static t_flags	f;

	ft_pf_f_init(&f);
	f.len = 0;
	va_start(f.args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (*format == '%')
				f.len += write(1, format++, 1);
			else
				format = ft_pf_type(&f, (char *)format) + 1;
		}
		else
			f.len += write(1, format++, 1);
	}
	va_end(f.args);
	return (f.len);
}

static void	type_init(t_flags *f, char *s)
{
	ft_pf_f_init(f);
	f->specifier_type = *s;
	f->string = s;
}

const char	*ft_pf_type(t_flags *f, char *s)
{
	type_init(f, s);
	if (ft_strchr("diuxXcsp", f->specifier_type))
		ft_diux(f);
	return ((const char *)s);
}

void	ft_pf_f_init(t_flags *f)
{
	f->string = NULL;
	if (!f->len)
		f->len = 0;
}

/* int main(void)
{
	int	len = 0, len_o = 0;

	len = ft_printf("%uq%u", 21447, 21648);
	puts("");
	len_o = printf("%uq%u", 21447, 21648);
	puts("");
	printf("Lenght ft_printf: %d\n", len);
	ft_printf("Lenght printf: %d\n", len_o);

	len =ft_printf("p: %p %p %p\n", &main, &printf, NULL);
	len_o = printf("p: %p %p %p\n", &main, &printf, NULL);
	printf("Lenght ft_printf: %d\n", len);
	ft_printf("Lenght printf: %d\n", len_o);
    len = ft_printf("x: %x %x %x %x\n", 0x123, 456, 0x789, 0xE9);
    len_o = printf("x: %x %x %x %x\n", 0x123, 456, 0x789, 0xE9);
 	printf("Lenght ft_printf: %d\n", len);
	ft_printf("Lenght printf: %d\n", len_o);

    len = ft_printf("X: %X %X %X %X\n", 0x123, 456, 0x789, 0xE9);
    len_o = printf("X: %X %X %X %X\n", 0x123, 456, 0x789, 0xE9);
	printf("Lenght ft_printf: %d\n", len);
	ft_printf("Lenght printf: %d\n", len_o);
	
	len = ft_printf("i: %i | %i\n", INT_MIN, INT_MAX);
	len_o = printf("i: %i | %i\n", INT_MIN, INT_MAX);

	printf("\nLenght ft_printf: %d\n", len);
	ft_printf("Lenght printf: %d\n\n", len_o);
	len = ft_printf("d: %d | %d\n", INT_MIN, INT_MAX);
	len_o = printf("d: %d | %d\n", INT_MIN, INT_MAX);

	printf("Lenght ft_printf: %d\n", len);
	ft_printf("Lenght printf: %d\n", len_o);

    len = ft_printf("Here are some basic conversions: \
		%c %s %p\n", 'A', "hello", &main);
    len_o = printf("Here are some basic conversions: \
		%c %s %p\n",'A', "hello", &main);
	
	printf("Lenght ft_printf: %d\n", len);
	ft_printf("Lenght printf: %d\n", len_o);

	len = ft_printf("%iq%i", 21447, -21648);
	puts("");
	len_o = printf("%iq%i", 21447, -21648);
	puts("");
	printf("Lenght ft_printf: %d\n", len);
	ft_printf("Lenght printf: %d\n", len_o);

	len = ft_printf("!%i%i%i%i%i%il\n", 0, 7, 0, 7, 0, 7);
	len_o = printf("!%i%i%i%i%i%il\n", 0, 7, 0, 7, 0, 7);
	printf("Lenght ft_printf: %d\n", len);
	ft_printf("Lenght printf: %d\n", len_o);

	len = ft_printf("|%d, |", 10); puts("\n");
	len_o = printf("|%d, |", 10); puts("\n");
	printf("Lenght ft_printf: %d\n", len);
	ft_printf("Lenght printf: %d\n", len_o);

	len = ft_printf("%d, %d\n", 10, 23);
	len_o = printf("%d, %d\n", 10, 23);
	printf("Lenght ft_printf: %d\n", len);
	ft_printf("Lenght printf: %d\n", len_o);
  	return (0);
} */
