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

size_t	ft_strlcat(char *dest, const char *src, size_t d_size)
{
	size_t	i;
	size_t	j;

	j = 0;
	i = ft_strlen(dest);
	if (i >= d_size)
		return (d_size + ft_strlen(src));
	while ((i + 1) < d_size && src[j] != '\0')
		dest[i++] = src[j++];
	dest[i] = '\0';
	return (ft_strlen(dest) + ft_strlen(&src[j]));
}
