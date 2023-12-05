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

size_t ft_intlen(int n)
{
    size_t len = 1;  // Começa com 1 para o caso de n ser 0

    if (n < 0)
    {
        len++;  // Conta o sinal negativo
        n = -n;  // Torna n positivo para facilitar a contagem dos dígitos
    }

    while (n > 9)
    {
        n /= 10;
        len++;
    }

    return len;
}

static void ft_inttochar(int n, size_t len, size_t i, char *str)
{
    str[len] = '\0';  // Adiciona o terminador nulo ao final da string
    while (len > i)
    {
        len--;
        str[len] = n % 10 + '0';
        n /= 10;
    }
}

char *ft_itoa(int n)
{
    size_t len;
    char *str;
    size_t i;

    i = 0;
    len = ft_intlen(n);
    str = (char *)malloc(sizeof(char) * (len + 1));
    if (!str)
        return NULL;

    if (n < 0)
    {
        str[0] = '-';
        n = -n;  // Torna n positivo para a conversão dos dígitos
        i = 1;
    }

    ft_inttochar(n, len, i, str);

    return str;
}

/*int main()
{
    printf("%s\n", ft_itoa(-2147483648));
}*/
