/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42porto.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:19:58 by rboia-pe          #+#    #+#             */
/*   Updated: 2023/04/07 02:19:58 by rboia-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isolate_line(char *str)
{
	int	i;
	int	j;
	int	checker;

	i = -1;
	j = -1;
	checker = 0;
	while (str[++i])
	{
		if (checker)
			str[++j] = str[i];
		if (str[i] == '\n')
			checker = 1;
		str[i] = '\0';
	}
	return (checker);
}

char	*ft_strjoin_get(char *s1, char *s2)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	str = malloc(ft_strlen_get(s1) + ft_strlen_get(s2) + 1);
	if (!str)
		return (NULL);
	if (s1 == 0 && s2 == 0)
		return (NULL);
	while (s1 && s1[j])
		str[i++] = s1[j++];
	j = 0;
	while (s2 && s2[j])
	{
		str[i] = s2[j++];
		if (str[i++] == '\n')
			break ;
	}
	str[i] = '\0';
	free(s1);
	return (str);
}

size_t	ft_strlen_get(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		i++;
	return (i);
}
