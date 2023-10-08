#include "../includes/so_long.h"

int quiser(int i, void *mlx_win);

void    init_struct(game_data *game)
{
    game->img = malloc(sizeof(t_data));
    if (game->img == NULL)

    // Inicialize cada membro de game.img
    game->img->img = NULL;
    game->img->addr = NULL;  // Certifique-se de inicializar todos os membros
	game->img->img = NULL;
	game->img->bits_per_pixel = 2000;
    game->img->line_length = 200;
    game->img->endian = 1500;
}

void	init_window(game_data *game)
{
	game->mlx_win = mlx_new_window(game->mlx, game->width / 2, game->height / 2, "Hello world!");
    game->img->img = mlx_new_image(game->mlx, game->width, game->height);
    game->img->addr = mlx_get_data_addr(game->img->img, &game->img->bits_per_pixel, &game->img->line_length,
                                &game->img->endian);
}

int check_file(char *map)
{
    int	fd;
	int	error;

	error = 0;
	fd = open(map, O_RDONLY);
	if (fd < 0)
		error = 1;
	map = ft_strrchr(map, '.');
	if (error == 0 && !map)
		error = 1;
	if (error == 0 && ft_strncmp(map, (const char *)".ber", 4) != 0)
		error = 1;
	if (error == 1)
		ft_printf("Error.\nThe file isn't valide\n");
	close (fd);
	return (error);
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
	
	col = -1;
	rows = -1;
	printf("%di\n", ft_strlen_get((const char *)(game->map)));
	while (game->map[++col])
	{
		while (game->map[ft_strlen_get((const char *)(game->map)) - 1][++rows] != '\n' && col == 0)
			if (game->map[0][rows] != '1')
			{
				ft_printf("daasd\n");
				return (1);
			}
		if (game->map[col][0] != '1' ||
			game->map[col][ft_strlen_get((const char *)(game->map))] != '1'){
				ft_printf("daasd\n");

				return (1);
			}
	}
	return (0);
}

int		check_pce(game_data *game)
{
	int	col;
	int	rows;
	int	count;
	int	count_e;
	
	col = 0;
	count = 0;
	count_e = 0;
	while (game->map[++col])
	{
		rows = 0;
		while (game->map[col][++rows])
		{
			if (game->map[col][rows] == 'P')
			{
				game->player->player_y = col;
				game->player->player_x = rows;
				count++;
			}
			else if (game->map[col][rows] == 'C')
				game->player->col_q++;
			if (game->map[col][rows] == 'E')
			{
				game->exit_y = col;
				game->exit_x = rows;
				count_e++;
			}
		}
	}
	if (count > 1 || count_e > 1)
		return (1);
	return (0);
}

/* int		check_valid_path(game_data *game)
{
/* 
• You have to check if there’s a valid path in the map.
• You must be able to parse any kind of map, as long as it respects the above rules.
 
	int	cols;
	int	rows;

	cols = 0;
	while (game->player)
}
 */

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
	
	col = -1;
	count = 0;
	while (game->map[++col])
	{
		rows = -1;
		while (game->map[col][++rows])
		{
			if (game->map[col][rows] != 'P' && game->map[col][rows] != 'C' && 
					game->map[col][rows] != 'E' && game->map[col][rows] != '1' && 
					game->map[col][rows] != '0')
				return (1);
		}
	}
	return (0);
}

int		check_rows_cols(game_data *game)
{
	game->rows = -1;
	game->col = -1;
	while (game->map[++game->rows] != NULL)
		;
	if (game->rows > 0)
		game->col = ft_strlen_get((const char *)game->map);
	if (game->col < 5 || game->rows < 3)
		return (1);
	return (0);
}

int main(int argc, char **argv)
{
    /* void *mlx;
    void *mlx_win; */
    game_data game;
    t_data *img;
	int	i;

	i = -1;
	if (argc != 2)
	{
		ft_printf ("Invalid number of arguments\n");
		return (1);
	}
    game.mlx = mlx_init();
	mlx_get_screen_size(game.mlx, &game.width, &game.height);
	init_struct(&game);
	if (check_file(argv[1]) != 0)
		exit (1);
	game.map = create_map(argv[1]);
	if (check_rectangular_map(&game) != 0 || check_pce(&game) != 0 || 
		 /* || check_obstacles_or_not(&game) != 0 ||*/ 
		check_rows_cols(&game) != 0)
	{
		ft_printf("Error.\nMap's invalid.\n");
		exit (1);
	}
	if (check_wall(&game) != 0)
	{
		ft_printf("EEEError.\nMap's invalid.\n");
		exit (1);
	}
	init_window(&game);




    // Imprima o texto na tela renderizada
    // mlx_string_put(game.mlx, game.mlx_win, (game.width / 2) / 2, (game.height / 2) / 2, 0xFF0000, "Hello, World!");


    mlx_hook(game.mlx_win, 17, 1L << 17, mlx_loop_end, game.mlx);
    mlx_key_hook(game.mlx_win, quiser, game.mlx);
	rendering(&game, "./textures/Sky.xpm", 512, 10);
    mlx_loop(game.mlx);
}

void rendering(game_data *game, char *path, int x, int y)
{
    if (game->img->img)
    {
        mlx_destroy_image(game->mlx, game->img->img);
        game->img->img = 0;
    }

    // Carregue a imagem do arquivo XPM
    game->img->img = mlx_xpm_file_to_image(game->mlx, path, &game->img->bits_per_pixel, &game->img->line_length);

    // Verifique se a imagem foi carregada com sucesso
    if (game->img->img == NULL)
    {
        fprintf(stderr, "Erro ao carregar a imagem do arquivo XPM.\n");
        exit(1);
    }

    // Renderize a imagem na janela
    mlx_put_image_to_window(game->mlx, game->mlx_win, game->img->img, x, y);
}

/* 
void put_tile(t_game *game, char *img, int x, int y)
{
    if (game->img.mlx_img)
    {
        mlx_destroy_image(game->mlx_ptr, game->img.mlx_img);
        game->img.mlx_img = 0;
    }
    game->img.mlx_img = mlx_xpm_file_to_image(game->mlx_ptr, img, &game->tile.x,
            &game->tile.y);
    mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, game->img.mlx_img, x, y);
}

 */

int quiser(int i, void *mlx_win)
{
    printf("%d\n", i);
    if (i == 65307)
        mlx_loop_end(mlx_win);
    if (i == 0xFF1B)
        return (i);
	return 0;
}
