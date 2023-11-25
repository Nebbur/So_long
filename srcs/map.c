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
	/*
• The map must be rectangular.
• The map must be closed/surrounded by walls. If it’s not, the program must return
an error.
*/
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
			{
				game->player->player_x = rows;
				game->player->player_y = col;
				count++;
			}
			else if (game->map[rows][col] == 'C')
				game->player->col_q++;
			else if (game->map[rows][col] == 'E')
			{
				game->exit_x = rows;
				game->exit_y = col;
				count_e++;
			}
		}
	}
	game->rows = rows - 1;
	if (count > 1 || count_e > 1)
		return (1);
	return (0);
}

int	dist_two_points(int x, int y, game_data *game)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	while (++i != x && ++j != y)
		;
	return (y);		
}

void	player_position_onthemap(game_data *game)
{
	player_st	*p;

	p = game->player;
	/* if (game->triggerY == 1)
	{
		ft_printf("PLAYER y %i\n", p->xy[1]);
		p->xy[0] = p->xy_last[0];
		p->xy[1] = p->xy_last[1];

		ft_printf("PLAYER y %i\n", p->xy[1]);
		game->triggerY = 0;
		game->trig = 0;
	} */
}

void	process_map(game_data *game, int trigger)
{
	int	i;
	int	j;
	int	pil;
	int	pic;
	int	trig;
	int checker;

	checker = 0;
	trig = 0;
	i = -1;
	ft_printf("\nProcess Map\n");

	ft_printf("\npx %i, py %i\n\n", game->player->xy[0], game->player->xy[1]);

	if (game->rl >= WINDOW_HEIGHT / BPX)
		pil = game->player->xy[0] - ((game->rl - 1) - 3); //(game->rl - 1 porque o rl comeca em 1 sendo que o pil deve comecar em 0)
	else
		pil = 0;	

	if (game->cl % 2 != 0)
	{
		pic = game->player->xy[1] - ((game->cl * 0.5) + 1);// + 1 porque o resultado de game->cl / 2 arredonda para o numero acima entao eu retiro esse acrescimo;
		ft_printf("Caso cl ímpar: pic = %i, cl %i\n", pic, game->cl);
		ft_printf("player Y %i\n", game->player->xy[1]);
	}
	else
	{
		pic = game->player->xy[1] - (game->cl * 0.5);
		ft_printf("Caso cl par: pic = %i\n", pic);
		ft_printf("player Y %i\n", game->player->xy[1]);
	}
	if (game->col >= WINDOW_WIDTH / BPX && game->cl - pic > 0) // RECOMECAR ESTE IF / CAUSA DO JUMPING DO PLAYER 
	{
		if (game->player->xy[1] == game->cl * 0.5)
		{
			game->sinal = 1; // emitiu sinal 4
		}
		if (game->player->xy[1] > game->cl * 0.5 || game->triggerY_temp == 1)
		{
			if (game->triggerY_temp == 1 && game->sinal == 1) // recebeu sinal 2 e 4
				game->triggerY_temp_2 = 1; // emitiu sinal 3 //o mapa foi atualizado
			game->triggerY_temp = 0;
			game->sinal = 0;
			ft_printf("y last %i\n", game->player->xy_last[1]);
			game->triggerY = 1;//emitiu sinal 1, triggerY_temp = 0 ----------- player encontra-se na coluna central
			ft_printf("Game col %i\n", game->col);
			pic = game->col - game->cl;
		}
		else
		{
			game->triggerY = 0;
			if (game->cl % 2 != 0)
			{
				if (game->pic != pic)
				{
					ft_printf("Last pic != actual pic\n");
					pic = game->player->xy[1] + 0 - ((game->cl * 0.5) + 1);// + 1 porque o resultado de game->cl / 2 arredonda para o numero acima entao eu retiro esse acrescimo;
					trig = 1;
					/* if (trig == 1)
					{
						game->trig = 1;
						game->triggerY = 1;
						//game->player->xy[1] = dist_two_points(game->player->xy[0], game->player->xy[1], game);
					} */
				}
				else
					pic = game->player->xy[1] - ((game->cl * 0.5) + 1);// + 1 porque o resultado de game->cl / 2 arredonda para o numero acima entao eu retiro esse acrescimo;
			}
			else
			{
				if (game->pic != pic)	
				{
					ft_printf("Last pic != actual pic\n");
					pic = game->player->xy[1] + 0 - (game->cl * 0.5);
					trig = 1;
					/* if (trig == 1)
					{
						game->trig = 1;
						game->triggerY = 1;
						//game->player->xy[1] = dist_two_points(game->player->xy[0], game->player->xy[1], game);
						ft_printf("player y%i\n", game->player->xy[1]);
					} */
				}
				else
					pic = game->player->xy[1] - (game->cl * 0.5);
			}
		}
	} //ESTE IF FAZ FUNCIONAR A MOVIMENTACAO DO MAPA MAS MUDA A POSICAO INICIAL DO PLAYER ////// é o contrario
	ft_printf("pic %i\n", pic);

	if (pil < 0 || game->rl < WINDOW_HEIGHT / BPX)
		pil = 0;
	if (pic < 0 || game->cl < WINDOW_WIDTH / BPX)
		pic = 0;
	if (game->rl > game->rows)
	{
		game->rl = game->rows;
		pil += (game->rows - game->rl);
		if (pil < 0)
			pil = 0;
	}

	if (game->cl >= game->col)
	{
		game->cl = game->col;
		pic += (game->col - game->cl);
		if (pic < 0)
			pic = 0;
	}

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

	if (trigger == 0)
	{
		game->player->xy[1] -= pic;
		game->player->xy[0] -= pil;
	}

	i = -1;
	while (game->visible_map[++i])
	{
		j = -1;
		while (++j < game->cl)
		{
			game->visible_map[i][j] = game->map[pil + i][pic + j];
			if (i == game->player->xy[0] && j == game->player->xy[1])
				if (trig == 1)
					trig = 0;
			else if (game->visible_map[i][j] == 'P')
				if (i != game->player->xy[0] || j != game->player->xy[1])
					game->visible_map[game->player->xy[0]][game->player->xy[1]] = '0';
			
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

	if (trigger == 1)
		game->map[game->player->player_x][game->player->player_y] = '0';

	game->pic = pic;
	game->pil = pil;
	if (game->triggerY == 1 && game->player->xy[1] >= game->cl * 0.5 - 1)//recebeu sinal 1
	{
		ft_printf("Sinal 1 recebido\n");
		ft_printf("PLAYER y %i\n", game->player->xy[1]);

		//armazenar o y_last neste if porque o  game->player->xy_last[1] é atualizado s
		int x_temp = game->player->xy[0];
		int y_temp = game->player->xy[1];
		if (game->player->xy_next[1] < game->cl * 0.5)
		{
			game->player->xy_next[0] = game->player->xy[0];
			game->player->xy_next[1] = game->player->xy[1];
		}
		if (game->player->xy_sto_last[1] < 0)
			game->player->xy_sto_last[1] = game->player->xy_last[1];
		game->player->xy_sto_last[0] = game->player->xy_last[0];
		game->player->xy[0] = game->player->xy_last[0];
		game->player->xy[1] = game->player->xy_last[1];
		
		ft_printf("PLAYER y %i\n", game->player->xy[1]);
		ft_printf("x %i, y %i\n", game->player->xy[0], game->player->xy[1]);
		ft_printf("x temp %i y temp %i\n", x_temp, y_temp);
		ft_printf("x next %i y next %i\n", game->player->xy_next[0], game->player->xy_next[1]);
		if (game->triggerY_temp_2 == 0)
		{
			ft_printf("Sinal 3 nao recebido\n");

			game->visible_map[game->player->xy[0]][game->player->xy[1]] = 'P';

			ft_printf("\tPLAYER y %i\n", game->player->xy[1]);

			game->player->xy[0] = x_temp;
			game->player->xy[1] = y_temp;
		}
		else //recebeu sinal 3
		{
			ft_printf("Sinal 3 recebido\n");
			ft_printf("PLAYER y second to last %i\n", game->player->xy_sto_last[1]);
			//mapa atualizado, agora é necessario atualizar a posicao do player porque neste momente ele está 1 bloco mais à frente do que onde deve-se encontrar

			game->player->xy[1] = game->player->xy_next[1];
			game->player->xy[0] = game->player->xy_next[0];
			game->player->xy_sto_last[0] = game->player->xy_next[0];
			game->player->xy_sto_last[1] = game->player->xy_next[1];

			game->sinal = 0;
			game->triggerY = 0;
			ft_printf("Sinal 1 %i\nSinal 2 %i\nSinal 3 %i\nSinal 4 %i\n", game->triggerY, 
			game->triggerY_temp, game->triggerY_temp_2, game->sinal);
			//game->visible_map[game->player->xy[0]][game->player->xy_last[1]] = 'P';
		}
		if (game->player->cont == 60)
		{
			game->visible_map[game->player->xy_next[0]][game->player->xy_next[1]] = 'P';
			game->player->cont = 0;
		}
		else
		{
			game->player->xy[0] = game->player->xy_sto_last[0];
			game->player->xy[1] = game->player->xy_sto_last[1];
			game->visible_map[game->player->xy_sto_last[0]][game->player->xy_sto_last[1]] = 'P';
		}
		game->player->cont++;
		game->triggerY = 0;
		game->triggerY_temp = 1; //emitiu sinal 2 -------------- as variaveis responsaveis pela posicao do player armazenam a sua posicao + 1 para o mapa processar o mapa como se o player estivesse na segunda metade do mapa
	}
	else
	{
		ft_printf("Sinal 1 nao recebido\n");
		game->visible_map[game->player->xy[0]][game->player->xy[1]] = 'P';
	}

}

void init_visible_map(game_data *game)
{
	int	i;

	i = -1;
	game->rl = WINDOW_HEIGHT / BPX;
	game->cl = WINDOW_WIDTH / BPX;
	if (game->rl > game->rows + 1)
		game->rl = game->rows + 1;
	ft_printf("linhas alocadas %i + 1", game->rl);
	game->visible_map = (char **)ft_calloc(game->rl + 1, sizeof(char *));
    ft_printf("game row %i, game col %i\n", game->rows, game->col);
	if (game->cl > game->col - 1)
		game->cl = game->col;
	//ft_printf("rl %i, cl %i\n", game->rl, game->cl);
	ft_printf("colunas alocadas %i + 1", game->cl);
	while (++i < game->rl)
	{
		//ft_printf("%i, ", i);
		game->visible_map[i] = (char *)ft_calloc(game->cl + 1, sizeof(char));
		if (game->visible_map[i] == NULL)
			ft_printf("ER\n");
	}
	//ft_printf("\nfinal da f() %i, ", i);
	game->visible_map[i] = NULL;
	ft_printf("game cl %i\n", game->cl); //exit(1);
	ft_printf("\n");
}

/* void init_visible_map(game_data *game, int trigger)
{
    int start_row;
    int end_row;
    int	i;
	int	row;

	row = -1;
    start_row = game->player->player_x - 8;
    end_row = start_row + 10;
	while (start_row < 0)
		start_row++;
	while (end_row >= game->height);
		end_row--;
	ft_printf("game height %i\n", game->height);
	ft_printf("player x %i\nplayer y %i\n", game->player->player_x, game->player->player_y);
	ft_printf("start row %i\nend row %i\n", start_row, end_row);
    if (start_row < 0 || end_row >= 15)
	{
        ft_printf("Intervalo de cópia inválido");
        return;
    }

	if (trigger == 0)
	{
		// Aloca memória para game->visible_map
		ft_printf("alocar memoria: %i\n", end_row );
		game->visible_map = (char **)ft_calloc(end_row, sizeof(char *));
		if (!game->visible_map)
		{
			ft_printf("Erro na alocação de memória");
			return;
		}
		i = -1;
		while (game->visible_map[++i])
			game->visible_map[i] = (char *)ft_calloc(ft_strlen(game->map[start_row + i] + 1), sizeof(char));
		game->visible_map[i] = NULL;
	}
	else
	{
		i = -1;
		while (game->visible_map[++i])
			game->visible_map[i] = game->map[start_row + i];
	}
	start_row--;
	while (++start_row <= end_row)
		game->visible_map[++row] = game->map[start_row];

	for (int i = 0; game->visible_map[i]; i++)
		for (int j = 0; game->visible_map[i][j]; j++)
	        ft_printf("%c", game->visible_map[i][j]);
	ft_printf("\n\n");
	for (int i = 0; game->map[i]; i++)
        ft_printf("%s", game->map[i]);
	i = -1;
	int j = -1;
	while (game->visible_map[++i])
	{
		j = -1;
		while (game->visible_map[i][++j])
			;
	}
	game->rows = i - 1;
	game->col = j;
	ft_printf("game rows %i\n game col %i\n", game->rows, game->col);

} */

int	init_map(char *path, game_data *game)
{
	if (check_file(path) != 0)
		exit (1);

	game->map = create_map(path);

	if (check_rectangular_map(game) != 0)
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
	game->player->xy[0] = game->player->player_x;
	game->player->xy[1] = game->player->player_y;
	process_map(game, 0);
	//game->map[game->player->player_x][game->player->player_y] = '0';
	//game->visible_map[game->player->player_x][game->player->player_y] = 'P';
	return (0);
}
