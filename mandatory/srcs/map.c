#include "../includes/so_long.h"

void	free_map(char **map)
{
	int	i;

	i = -1;
	while (map[++i])
		free(map[i]);
	free(map);
}

char	**create_map(char *path)
{
	int		fd;
	int		i;
	int		j;
	char	**map;

	i = get_lines(path);
	map = (char **)calloc(i + 1, sizeof(char *));
	if (!map)
		return (NULL);
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		free(map); // Liberar map antes de retornar em caso de erro
		close (fd);
		return (NULL);
	}
	j = -1;
	while (++j != i)
	{
		map[j] = get_next_line(fd);
		if (!map[j])
		{
			free_map(map); // Liberar map em caso de erro
			map[j] = NULL;
			close(fd);
			return (NULL);
		}
	}
	map[j] = NULL;
	close(fd);

	return (map);
}

int	check_obstacles_or_not(game_data *game)
{
	int	col;
	int	rows;
	int	count;
	char **m;

	m = game->map;
	rows = -1;
	count = 0;
	while (rows < game->rows && m[++rows])
	{
		col = -1;
		while (col < game->col - 1 && m[rows][++col])
		{
			if (!(ft_strchr("PCE10", m[rows][col])))
				return (1);
			else if (m[rows][col] == 'E')
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

/* int	check_path(game_data *game)
{
	ft_printf("C position %i %i\n", game->player->collectable_pos[0], game->player->collectable_pos[1]);
	ft_printf("P position %i %i\n", game->player->player_pos[0], game->player->exit_pos[1]);
	ft_printf("E position %i %i\n", game->player->exit_pos[0],game->player->exit_pos[1]);
	int	rows;
	int	col;


	
	exit(1);
} */

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
		return (1);	
	canCollectAll(map_copy, game, game->player->init_xy[0], game->player->init_xy[1]);
	if (game->player->col_q != game->player->col_collected)
	{
		free_map(map_copy);
		return (1);
	}
	game->player->col_collected = 0;
	free_map(map_copy);
	map_copy = create_map(str);
	canReachExit(map_copy, game, game->player->init_xy[0], game->player->init_xy[1]);
	if (map_copy[game->player->exit_pos[0]][game->player->exit_pos[1]] != 'O')
	{
		free_map(map_copy);
		return (1);	
	}
	free_map(map_copy);
	return (0);
}

/*
• The map must be rectangular.
• The map must be closed/surrounded by walls. If it’s not, the program must return
an error.
*/
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
	int error;

	error = 0;
	col = -1;
	rows = -1;
	while (game->map[++rows])
	{
		col = -1;
		while ((rows == 0 || rows == game->rows) && ++col < game->col)
		{
			if (game->map[rows][col] != '1')
				error = 1;
		}
		if (game->map[rows][0] != '1' || game->map[rows][game->col - 1] != '1')
				error = 1;
	}
	if (error == 1)
		ft_printf("Error. Invalid wall.\n");
	return (error);
}

static void	aux_check_pce(int xy[2], int r_c[2], int *count, char c, game_data * game)
{
	if (game->map[r_c[0]][r_c[1]] == c)
	{
		*&xy[0] = r_c[0];
		*&xy[1] = r_c[1];
		*count += 1;
	}
}

int		check_pce(game_data *game)
{
	int	r_c[2];		//rows 0 - cols 1
	int	cp_ce[2];	//count player 0 - count exit 1

	r_c[0] = 0;
	cp_ce[0] = 0;
	cp_ce[1] = 0;
	while (game->map[++r_c[0]])
	{
		r_c[1] = 0;
		while (game->map[r_c[0]][++r_c[1]])
		{
			aux_check_pce(game->player->init_xy, r_c, &cp_ce[0], 'P', game);
			aux_check_pce(game->exit, r_c, &cp_ce[1], 'E', game);
			if (game->map[r_c[0]][r_c[1]] == 'C')
				game->player->col_q++;
		}
	}
	game->rows = r_c[0] - 1;
	if (cp_ce[0] != 1 || cp_ce[1] != 1)
		return (1);
	return (0);
}

int	dist_two_points(int from_y, int to_y, game_data *game)
{
	int	i;

	i = 0;
	from_y--;
	to_y;
	while (++from_y != to_y)
		i++;
		ft_printf("dist two points %i\n", i);
	return (i);		
}

void	player_position_onthemap(game_data *game)
{
	player_st	*p;

	p = game->player;
}

void	aux_find_pic(game_data *game, int pic, int trigger)
{
	player_st	*p;
	int			l;

	p = game->player;
	if (trigger == 1)
		l = game->cl;
	else
		l = game->rl;
	if (game->cl % 2 != 0)
		pic = p->xy[1] + 0 - ((l * 0.5) + 1);// + 1 porque o resultado de game->cl / 2 arredonda para o numero acima entao eu retiro esse acrescimo;
	else
		pic = p->xy[1] + 0 - (l * 0.5);
}

void	process_map(game_data *game)
{
	int	i;
	int	j;
	int	pil;
	int	pic;
	player_st *p;

	p = game->player;
	i = -1;

	ft_printf("\nProcess Map\n");
	ft_printf("\npx %i, py %i\n\n", p->xy[0], p->xy[1]);

	if (game->rl >= WINDOW_HEIGHT / BPX)
		pil = p->xy[0] - ((game->rl - 1) - 3); //(game->rl - 1 porque o rl comeca em 1 sendo que o pil comeca em 0)
	else
		pil = 0;	
	aux_find_pic(game, pil, 0);
	aux_find_pic(game, pic, 1);
	if (game->col >= WINDOW_WIDTH / BPX && p->xy[1] > game->cl * 0.5)
	{
		pic = p->xy[1] - game->cl * 0.5;
		if (pic > game->col - game->cl)
			pic = game->col - game->cl;
	}
	else if (!(p->xy[1] > game->cl * 0.5))
		aux_find_pic(game, pic, 1);

	ft_printf("WINDOWS HEIGHT / BPX %i\n", WINDOW_HEIGHT / BPX);
	if (pil < 0 || game->rl < WINDOW_HEIGHT / BPX)
		pil = 0;
	ft_printf("WINDOW_WIDTH / BPX %i\n", WINDOW_WIDTH / BPX);
	if (pic < 0 || game->cl < (WINDOW_WIDTH / BPX) - 1)
		pic = 0;
	if (game->rl >= game->rows)
	{
		game->rl = game->rows;
		//pil += (game->rows - game->rl);
		if (pil < 0)
			pil = 0;
	}

	if (pil > (game->rows + 1) - game->rl)
			pil = (game->rows + 1 )- game->rl;

	if (game->cl >= game->col)
		game->cl = game->col;

	ft_printf("\nrows %i col %i", game->rows, game->col);
	ft_printf("\npil %i, pic %i\nrl %i, cl %i\n", pil, pic, game->rl, game->cl);
	ft_printf("\nORIGINAL MAP:\n");
	while (game->visible_map[++i])
	{
		j = -1;
		while (++j < game->cl)
			ft_printf("%c", game->map[pil + i][pic + j]);
		if (game->map[i][pic + j - 1] != '\n')
			ft_printf("\n");
	}

	ft_printf("player y %i\n", p->xy[1]);
	ft_printf("pil %i\n", pil);
	i = -1;
	while (game->visible_map[++i])
	{
		j = -1;
		ft_printf("pil + i\n%i + %i\n", pil, i);	
		while (++j < game->cl)
		{
			game->visible_map[i][j] = game->map[pil + i][pic + j];
		}

		game->visible_map[i][j] = '\n';
	}

	i = -1;
	ft_printf("\nVISIBLE MAP:\n");
	while (game->visible_map[++i])
	{
		j = -1;
		while (++j <= game->cl)
			ft_printf("%c", game->visible_map[i][j]);
	}

	game->pic = pic;
	game->pil = pil;
}

void init_visible_map(game_data *game)
{
	int	i;

	i = -1;
	game->rl = WINDOW_HEIGHT / BPX;
	game->cl = WINDOW_WIDTH / BPX;
	if (game->rl > game->rows + 1)
		game->rl = game->rows + 1;
	game->visible_map = (char **)ft_calloc(game->rl + 1, sizeof(char *));
	if (game->visible_map == NULL)
		exit(1); //ENCAMINHAR PARA FUNCAO EXIT
	if (game->cl > game->col - 1)
		game->cl = game->col;
	while (++i < game->rl)
	{
		game->visible_map[i] = (char *)ft_calloc(game->cl + 1, sizeof(char));
		if (game->visible_map[i] == NULL)
			exit(1); //ENCAMINHAR PARA FUNCAO EXIT
	}
	game->visible_map[i] = NULL;
}


int	init_map(char *path, game_data *game)
{
	if (check_file(path) != 0)
		exit (1);

	game->map = create_map(path);

	if (check_rectangular_map(game))
		exit(1);
	if (check_pce(game) != 0)
		exit(1);
	if (check_rows_cols(game) != 0)
		exit(1);
	if (check_wall(game) != 0)
		exit(1);
	if (check_obstacles_or_not(game) != 0)
		exit(1);
	if (path_check(path, game) != 0)
		exit(1);
	/* if (check_rectangular_map(game) != 0 || check_pce(game) != 0 || 
		check_rows_cols(game) != 0 || check_wall(game) != 0 ||
		check_obstacles_or_not(game) != 0 || path_check(path, game) != 0)
	{
		ft_printf("Error.\nMap's invalid.\n");
		exit (1);
	} */
	game->height = game->rows;
	game->width = game->col;
	init_visible_map(game);
	game->player->xy[0] = game->player->init_xy[0];
	game->player->xy[1] = game->player->init_xy[1];
	process_map(game);
	return (0);
}
