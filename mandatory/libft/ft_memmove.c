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

static void	*ft_revmemcpy(void *dest, const void *src, size_t num)
{
	while (num-- > 0)
		((unsigned char *)dest)[num] = ((unsigned char *)src)[num];
	return (dest);
}

void	*ft_memmove(void *dest, const void *src, size_t num)
{
	if (((unsigned char *)dest) == ((unsigned char *)src))
		return (dest);
	else if (dest < src)
		ft_memcpy(dest, src, num);
	else
		ft_revmemcpy(dest, src, num);
	return (dest);
}
