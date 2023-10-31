#include "checker.h"

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

char	*get_next_line(int fd)
{
	static char		buf[BUFFER_SIZE + 1];
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = NULL;
	while (buf[0] || read(fd, &buf, BUFFER_SIZE) > 0)
	{
		line = ft_strjoin_get(line, buf);
		if (ft_strlen_get(buf) == 0)
			break ;
		if (ft_isolate_line(buf) == 1)
			break ;
		if (read(fd, &buf, BUFFER_SIZE) < 0)
		{
			free (line);
			return (NULL);
		}
	}
	return (line);
}

int	get_lines(char *map)
{
	int		fd;
	int		i;
	char	*line;
	
	i = 0;
	fd = open(map, O_RDONLY);
	if (fd < 0)
		return (1);
	line = get_next_line(fd);
	while (line && i++ >= 0)
	{
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	free(line);
	return (i);
}

void	free_map(char **map)
{
	int	i;

	i = -1;
	while (map[++i])
		free(map[i]);
	free(map[i]);
}

char	**create_map(char *path)
{
	int		fd;
	int		i;
	int		j;
	char	**map;

	i = get_lines(path);
	map = (char **)malloc(sizeof(char *) * (i + 1));
	if (!map)
		return (NULL);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	j = -1;
	while (++j != i)
	{
		map[j] = get_next_line(fd);
		if (!map[j])
		{
			free_map(map);
			return (NULL);
		}
	}
	map[j] = NULL;
	close (fd);

	return (map);
}

int	check_obstacles_or_not(game_data *game)
{
	int	col;
	int	rows;
	int	count;
	
	rows = -1;
	count = 0;
	while (rows < game->rows && game->map[++rows])
	{
		col = -1;
		while (col < game->col && game->map[rows][++col])
		{
			if (game->map[rows][col] == '\n')
				break;
			if (game->map[rows][col] != 'P' && game->map[rows][col] != 'C' && 
					game->map[rows][col] != 'E' && game->map[rows][col] != '1' && 
					game->map[rows][col] != '0')
				return (1);
			else if (game->map[rows][col] == 'P')
			{
				game->player->player_pos[0] = rows;
				game->player->player_pos[1] = col;
			}
			else if (game->map[rows][col] == 'C')
			{
				game->player->collectable_pos[0] = rows;
				game->player->collectable_pos[1] = col;
			}
			else if (game->map[rows][col] == 'E')
			{
				game->player->exit_pos[0] = rows;
				game->player->exit_pos[1] = col;
			}
		}
	}
	return (0);
}

int		check_rows_cols(game_data *game)
{
	game->col = -1;
	while (game->map[++game->rows])
		;
	game->rows--;
	if (game->rows > 0)
		while (game->map[game->rows][++game->col] != '\0')
			;
	game->col--;
	if (game->col < 5 || game->rows < 3)
		return (1);

	return (0);
}

void	canCollectAll(char **map_copy, game_data *game, int y, int x)
{
	if (x < 1 || y < 1 || x > game->col || y > game->rows)
		return ;
	if (map_copy[y][x] == '\0' || map_copy[y][x] == '\n')
		return ;
	if (map_copy[y][x] == 'O' || map_copy[y][x] == '1')
		return ;
	if (map_copy[y][x] == 'E')
		return ;
	if (map_copy[y][x] == 'C')
		game->player->col_collected++;
	map_copy[y][x] = 'O';
	canCollectAll(map_copy, game, y - 1, x);
	canCollectAll(map_copy, game, y + 1, x);
	canCollectAll(map_copy, game, y, x - 1);
	canCollectAll(map_copy, game, y, x + 1);
}

void	canReachExit(char **map_copy, game_data *game, int y, int x)
{
	if (x < 1 || y < 1 || x > game->col || y > game->rows)
		return ;
	if (map_copy[y][x] == '\0' || map_copy[y][x] == '\n')
		return ;
	if (map_copy[y][x] == 'O' || map_copy[y][x] == '1')
		return ;
	map_copy[y][x] = 'O';
	canReachExit(map_copy, game, y - 1, x); 
	canReachExit(map_copy, game, y + 1, x); 
	canReachExit(map_copy, game, y, x - 1); 
	canReachExit(map_copy, game, y, x + 1);
}

static int	path_check(char *str, game_data *game)
{
	char	**map_copy;

	map_copy = create_map(str);
	if (!map_copy)
	{
		free_map(map_copy);
		return (1);	
	}
	canCollectAll(map_copy, game, game->player->player_pos[0], game->player->player_pos[1]);
	if (game->player->col_q != game->player->col_collected)
	{
		free_map(map_copy);
		return (1);
	}
	game->player->col_collected = 0;
	free_map(map_copy);
	map_copy = create_map(str);
	canReachExit(map_copy, game, game->player->player_pos[0], game->player->player_pos[1]);
	if (map_copy[game->player->exit_pos[0]][game->player->exit_pos[1]] != 'O')
	{
		free_map(map_copy);
		return (1);	
	}
	free_map(map_copy);
	return (0);
}

int	check_rectangular_map(game_data *game)
{
	int	rows;

	rows = -1;
	while (game->map[++rows])
		if (ft_strlen_get((const char*)game->map[0]) != ft_strlen_get((const char *)game->map[rows]))
			return (1);
	return (0);
}

int		check_wall(game_data *game)
{
	int	col;
	int	rows;
	
	col = -1;
	rows = -1;

	while (game->map[++col])
		while (rows < game->rows && game->map[ft_strlen_get((const char *)(game->map)) - 1][++rows] != '\n' && col == 0)
			if (game->map[col][0] != '1' || game->map[0][rows] != '1' ||
			game->map[col][ft_strlen_get((const char *)(game->map)) - 1] != '1')
				return (1);
	return (0);
}

int		check_pce(game_data *game)
{
	int	col;
	int	rows;
	int	count;
	int	count_e;
	
	rows = 0;
	count = 0;
	count_e = 0;
	while (game->map[++rows])
	{
		col = 0;
		while (game->map[rows][++col])
		{
			if (game->map[rows][col] == 'P')
				count++;
			else if (game->map[rows][col] == 'C')
				game->player->col_q++;
			else if (game->map[rows][col] == 'E')
				count_e++;
		}
	}
	game->rows = rows - 1;
	if (count > 1 || count_e > 1)
		return (1);
	return (0);
}

int	init_map(char *path, game_data *game)
{
	game->player = malloc(sizeof(player_st));
	game->player->col_collected = 0;
	game->player->col_q = 0;
	game->map = create_map(path);

	if (check_rectangular_map(game) != 0)
		return (1);
	if (check_pce(game) != 0)
		return (2);
	if (check_rows_cols(game) != 0)
		return (3);
	if (check_wall(game) != 0)
		return (4);
	if (check_obstacles_or_not(game) != 0)
		return (5);
	if (path_check(path, game) != 0)
		return (6);
	return (0);
}


int main(int argc, char **argv)
{
    game_data game;
    return (init_map(argv[1], &game));
}