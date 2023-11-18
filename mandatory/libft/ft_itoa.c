/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42porto.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:19:58 by rboia-pe          #+#    #+#             */
/*   Updated: 2023/04/07 02:19:58 by rboia-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_intlen(int n)
{
	long int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static void	ft_inttochar(int n, int len, int i, char *str)
{
	str[len] = '\0';
	while (i + 1 < len)
	{
		str[len - 1] = n % 10 + 48;
		n /= 10;
		len--;
	}
}

char	*ft_itoa(int n)
{
	size_t	len;
	char	*str;
	int		i;

	i = -1;
	len = ft_intlen(n);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	if (n == -2147483648)
	{
		str[0] = '-';
		str[1] = '2';
		n = 147483648;
		i = 1;
	}
	else if (n < 0)
	{
		str[0] = '-';
		n *= -1;
		i = 0;
	}
	ft_inttochar(n, len, i, str);
	return (str);
}

/*int main()
{
    printf("%s\n", ft_itoa(-2147483648));
}*/
