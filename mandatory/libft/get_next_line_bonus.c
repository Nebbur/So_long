/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42porto.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:19:58 by rboia-pe          #+#    #+#             */
/*   Updated: 2023/04/07 02:19:58 by rboia-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*get_next_line_bonus(int fd)
{
	static char		buf[FOPEN_MAX][BUFFER_SIZE + 1];
	char			*line;

	if (fd < 0 || FOPEN_MAX < fd)
		return (NULL);
	line = NULL;
	while (buf[fd][0] || read(fd, buf[fd], BUFFER_SIZE) > 0)
	{
		line = ft_strjoin_get(line, buf[fd]);
		if (ft_strlen_get(buf[fd]) == 0)
			return (line);
		if (ft_isolate_line(buf[fd]) == 1)
			break ;
		if (read(fd, buf[fd], BUFFER_SIZE) < 0)
		{
			free (line);
			return (NULL);
		}
	}
	return (line);
}

/*int	main(int ac, char **av)
{
	char	*line;
	int		i;
	int		fd;
	int		fd1;
	int		fd2;
	int		fd3;

	fd1 = open("tests/test.txt", O_RDONLY);
	fd2 = open("tests/test2.txt", O_RDONLY);
	fd3 = open("tests/test3.txt", O_RDONLY);

	i = 1;
	if (ac > 1)
		fd = open(av[1], O_RDONLY);
	while (i < 9)
	{
		line = get_next_line(fd);
		printf("line [%02d]: %s", i, line);
		free(line);
		line = get_next_line(fd2);
		printf("line [%02d]: %s", i, line);
		free(line);
		line = get_next_line(fd3);
		printf("line [%02d]: %s", i, line);
		free(line);
		i++;
	}
	puts("\n");
	if (ac > 1)
		close(fd);
	close(fd1);
	close(fd2);
	close(fd3);
	return (0);
}*/
