/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_aux.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42porto.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:19:58 by rboia-pe          #+#    #+#             */
/*   Updated: 2023/04/07 02:19:58 by rboia-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		ft_putchar(str[i]);
	return (ft_strlen(str));
}

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

char	*ft_strchr(const char *str, int d)
{
	while (*str != '\0' && *str != (char)d)
		str++;
	if (*str == (char)d)
		return (&*((char *)str));
	return (0);
}

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 != '\0')
		if (*s1++ != *s2)
			return (*--s1 - *s2);
	return (0);
}
