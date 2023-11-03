#include "../includes/so_long.h"

int quiser(int i, void *mlx_win);

void    init_struct(game_data *game)
{
	game->img = calloc(sizeof(t_data), 1);
	if (game->img == NULL)
	{
		free(game->img);
		ft_printf("Error.\n Nao foi possivel realizar alocaçao de memoria");
		exit(EXIT_FAILURE);
	}
	game->player = calloc(sizeof(player_st), 1);
	if (game->player == NULL)
	{
		free (game->player);
		ft_printf("Error.\n Nao foi possivel realizar alocaçao de memoria");
		exit(EXIT_FAILURE);
	}
	game->map = NULL;
	game->img->img = NULL;
	game->img->bits_per_pixel = 2000;
	game->img->line_length = 200;
	game->img->endian = 1500;
	game->po[0] = 0;
	game->po[1] = 0;
	game->lm = 0;
	game->sprites = ft_calloc(1, sizeof(t_sprites));
	if (!game->sprites)
	{
		free(game->sprites);
		exit(EXIT_FAILURE);
	}
	game->coin = ft_calloc(sizeof(t_coin), 1);
	if (!game->coin)
	{
		free(game->coin);
		exit(EXIT_FAILURE);
	}
	game->coin->max_frame = MCF;
	game->coin->current_frame = 0;
	game->fps = 0;

	/* game->width = 0;
	game->height = 0; */
	game->col = 0;
	game->rows = 0;
	game->exit_x = 0;

	game->player->player_x = 0;
	game->player->player_y = 0;
	game->player->player_pos[0] = 0;
	game->player->player_pos[1] = 0;
	game->player->col_collected = 0;
	game->player->col_q = 0;
	game->player->collectable_pos[0] = 0;
	game->player->collectable_pos[1] = 0;
	
}

void	init_window(game_data *game)
{
	game->mlx_win = mlx_new_window(game->mlx, game->col / 2, game->rows / 2, "So long");
	game->img->img = mlx_new_image(game->mlx, game->col, game->rows + 1);
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

int get_lines(char *map)
{
    int fd;
    int i;
    char *line;

    i = 0;
    fd = open(map, O_RDONLY);
    if (fd < 0)
        return -1;  // Retorna um valor negativo para indicar erro

    while ((line = get_next_line(fd)) != NULL)
    {
        free(line);
        i++;
    }

    close(fd);
    return i;
}

/*
s_type = sprite_type | define se o sprite é um tile_lava, player, etc..
	1 = player
	2 = tile_rock
	3 = tile_lava
*/

char	*path_sprite(char *s_name, int i)
{
	char	*nbr;
	char	*str1;
	char	*str2;

	nbr = ft_itoa(i);
	str1 = ft_strjoin("./textures/", s_name);
	str2 = ft_strjoin(str1, "/");
	if (str1 != NULL)
		free(str1);
	str1 = ft_strjoin(str2, s_name);
	if (str2 != NULL)
		free(str2);
	str2 = ft_strjoin(str1, "_");
	if (str1 != NULL)
		free(str1);
	str1 = ft_strjoin(str2, nbr);
	if (str2 != NULL)
		free(str2);
	str2 = ft_strjoin(str1, ".xpm");
	if (str1 != NULL)
		free(str1);
	if (nbr != NULL)
		free(nbr);
	return (str2);
}

void	load_sprite(char *s_name, int s_nbr, int s_type, game_data *game)
{ 
	char		*path;
	int			i;
	int			fd;
	int			d;

	d = BPX;
	i = -1;
	while (++i < s_nbr)
	{
		path = path_sprite(s_name, (i + 1));
		fd = open(path, O_RDONLY);
		if (fd < 0)
		{
			ft_printf("Erro.\nNao foi possivel abrir uma imagem\n");
			free(path);
			exit (EXIT_SUCCESS);
		}
		close(fd);
		if (s_type == 1)
			game->sprites->p[i] = mlx_xpm_file_to_image(game->mlx, path, &d, &d);
		else if (s_type == 2)
			game->sprites->tr[i] = mlx_xpm_file_to_image(game->mlx, path, &d, &d);
		else if (s_type == 3)
			game->sprites->tl[i] = mlx_xpm_file_to_image(game->mlx, path, &d, &d);
		else if (s_type == 4)
			game->sprites->h[i] = mlx_xpm_file_to_image(game->mlx, path, &d, &d);
		else if (s_type == 5)
			game->sprites->bg[i] = mlx_xpm_file_to_image(game->mlx, path, &d, &d);
		else if (s_type == 6)
			game->sprites->gt[i] = mlx_xpm_file_to_image(game->mlx, path, &d, &d);
		else if (s_type == 7)
			game->sprites->c[i] = mlx_xpm_file_to_image(game->mlx, path, &d, &d);
		free(path);
	}
}

static void	destroy_sprite(char *s_name, int s_nbr, int s_type, game_data *game)
{ 
	char		*path;
	int			i;
	int			fd;
	int			d;

	d = BPX;
	i = -1;
	while (++i < s_nbr)
	{
		path = path_sprite(s_name, (i + 1));
		fd = open(path, O_RDONLY);
		if (fd < 0)
		{
			ft_printf("Erro.\nNao foi possivel abrir uma imagem\n");
			free(path);
			exit (EXIT_SUCCESS);
		}
		close(fd);
		if (s_type == 1)
			mlx_destroy_image(game->mlx, game->sprites->p[i]);
		else if (s_type == 2)
			mlx_destroy_image(game->mlx, game->sprites->tr[i]);
		else if (s_type == 3)
			mlx_destroy_image(game->mlx, game->sprites->tl[i]);
		else if (s_type == 4)
			mlx_destroy_image(game->mlx, game->sprites->h[i]);
		else if (s_type == 5)
			mlx_destroy_image(game->mlx, game->sprites->bg[i]);
		else if (s_type == 6)
			mlx_destroy_image(game->mlx, game->sprites->gt[i]);
		else if (s_type == 7)
			mlx_destroy_image(game->mlx, game->sprites->c[i]);
		free(path);
	}
}

void	destroy_sprites(game_data *game)
{
	destroy_sprite("player", 1, 1, game);
	destroy_sprite("tile_rock", 37, 2, game);
	destroy_sprite("lava_tile", 14, 3, game);
	destroy_sprite("heart", 2, 4, game);
	destroy_sprite("background", 1, 5, game);
	destroy_sprite("gate", 1, 6, game);
	destroy_sprite("coin", 9, 7, game);
}

void	init_sprites(game_data *game)
{
	load_sprite("player", 1, 1, game);
	load_sprite("tile_rock", 37, 2, game);
	load_sprite("lava_tile", 14, 3, game);
	load_sprite("heart", 2, 4, game);
	load_sprite("background", 1, 5, game);
	load_sprite("gate", 1, 6, game);
	load_sprite("coin", 9, 7, game);
}

void	init_camera(game_data *game)
{
	game->go[0] = 0;
	game->go[1] = 0;
	if (game->width < 15)
		game->go[0] = (14 - game->width) / 2 * BPX + 30;
	if (game->height < 8)
		game->go[1] = (7 - game->height) / 2 * BPX + 35;
}

// Create enmies in a linked list
void	create_enemy(int row, int col, game_data *game)
{
	t_enemy	*e;

	e = ft_calloc(sizeof(t_enemy), 1);
	if (e == NULL)
		exit(EXIT_FAILURE);
	e->pp[0] = col * BPX;
	e->pp[1] = row * BPX;
	e->init_p[0] = e->pp[0];
	e->init_p[1] = e->pp[1];
	e->alive = 1;
	e->direction = 1;
	e->move = 1;
	e->ai = 0;
	e->td = 0;
	if (game->enemies == NULL)
		game->enemies = ft_lstnew(e);
	else
		ft_lstadd_front(&game->enemies, ft_lstnew(e));
}

void	init_enemies(game_data *game)
{
	int	rows;
	int	cols;

	rows = -1;
	game->e = 0;
	while (game->map[++rows])
	{
		cols = -1;
		while (game->map[rows][++cols])
		{
			if (game->map[rows][cols] == 'M')
			{
				game->e++;
				create_enemy(rows, cols, game);
			}
		}
	}
}

int	get_action_keycode(int keycode)
{
	if (keycode == LETTER_KEY_LEFT || keycode == ARROW_KEY_LEFT)
		return (1);
	else if (keycode == LETTER_KEY_RIGHT || keycode == ARROW_KEY_RIGHT)
		return (2);
	else if (keycode == LETTER_KEY_UP || \
	keycode == ARROW_KEY_UP || keycode == SPACE_BAR)
		return (10);
	else if (keycode == LETTER_KEY_DOWN || \
	keycode == ARROW_KEY_DOWN)
		return (4);
	else if (keycode == LETTER_KEY_ACTION)
		return (5);
	return (0);
}

void	unset_action(int keycode, game_data *game)
{
	player_st	*p;

	p = game->player;
	if (p->ac[0] == get_action_keycode(keycode))
		p->ac[0] = 0;
	if (p->ac[1] == get_action_keycode(keycode))
		p->ac[1] = 0;
	if (p->ac[0] == 0 && p->ac[1] != 0)
	{
		p->ac[0] = p->ac[1];
		p->ac[1] = 0;
	}
	if (p->ac[2] == get_action_keycode(keycode))
		p->ac[2] = 0;
}

// set action
void	set_action(int keycode, game_data *game)
{
	player_st	*p;

	p = game->player;
	if (get_action_keycode(keycode) != 5)
	{
		if (p->ac[0] == 0 ||
			p->ac[0] == get_action_keycode(keycode))
			p->ac[0] = get_action_keycode(keycode);
		else if (p->ac[1] == 0 || 
			p->ac[1] == get_action_keycode(keycode))
			p->ac[1] = get_action_keycode(keycode);
	}
	else
		p->ac[2] = get_action_keycode(keycode);
	if (p->ac[0] == 1 || p->ac[1] == 1)
		p->d = 0;
	if (p->ac[0] == 2 || p->ac[1] == 2)
		p->d = 1;
}

int	keydown_hook(int keycode, game_data *game)
{
	set_action(keycode, game);
	return (0);
}

int	keyup_hook(int keycode, game_data *game)
{
	if (keycode != 53 && keycode != 65307)
		unset_action(keycode, game);
	else
		close_hook(0, game);
	return (0);
}

int	close_hook(int keycode, game_data *game)
{
	if (keycode == 0)
	{
		mlx_destroy_window(game->mlx, game->mlx_win);
	}
	return (0);
}

void	hook_register(game_data *game)
{
	mlx_hook(game->mlx_win, 17, 1L << 0, close_hook, game);
	mlx_hook(game->mlx_win, 2, 1L << 0, keydown_hook, game);
	mlx_hook(game->mlx_win, 3, 1L << 1, keyup_hook, game);
}

void	show_debug(game_data *game)
{
	/* mlx_put_image_to_window(game->mlx, game->mlx_win, \
	game->sprites->tr[5], 10, 10); */
	show_fps(game);
	//show_action(game);
}

void	draw_block(int x, int y, void *sprite, game_data *game)
{
	if (x < -BPX || y < -BPX || x > WINDOW_WIDTH || y > WINDOW_HEIGHT)
		return ;
	mlx_put_image_to_window(game->mlx, game->mlx_win,
		sprite, x + game->go[0], y + game->go[1]);
}

static void	background(game_data *game)
{
	int	rows;
	int	cols;

	rows = -1;
	while (++rows <= game->rows)
	{
		cols = -1;
		while (++cols < game->col)
		{
			if (game->map[rows][cols] == '0' || game->map[rows][cols] == 'M'
			|| game->map[rows][cols] == 'P')
				draw_block(cols * BPX + game->po[0],
					rows * BPX + game->po[1], game->sprites->bg[0],
					game);
		}
	}
}

// Setting player collision points
void	player_position(game_data *game)
{
	player_st	*p;

	p = game->player;
	p->tll[0] = p->pp[0];
	p->tll[1] = p->pp[1];
	p->tr[0] = p->tll[0];
	p->tr[1] = p->tll[1] + BPX;
	p->bl[0] = p->tll[0] + BPX;
	p->bl[1] = p->tll[1];
	p->br[0] = p->tll[0] + BPX;
	p->br[1] = p->tll[1] + BPX;
}

void	show_hud(game_data *game)
{
	//mlx_put_image_to_window(game->mlx, game->mlx_win, game->sprites->h[0], \
	//HUD_LM + 40, 40);
	//show_movement(so_long);
	//show_life(so_long);
}

void	wall(game_data *game)
{
	int	rows;
	int	cols;

	rows = -1;
	while (++rows <= game->rows)
	{
		cols = -1;
		while (++cols < game->col)
		{
			if (game->map[rows][cols] == '1')
			{
				if (rows != 0 && game->map[rows - 1][cols] == '1' &&
					game->map[rows][cols - 1] == '1' && game->map[rows - 1][cols - 1] != '1')
					draw_block(cols * BPX + game->po[0],
						rows * BPX + game->po[1], game->sprites->tr[12],
						game);
				else if (rows != 0 && game->map[rows - 1][cols] == '1' &&
					game->map[rows][cols + 1] == '1' && game->map[rows - 1][cols + 1] != '1')
					draw_block(cols * BPX + game->po[0],
						rows * BPX + game->po[1], game->sprites->tr[11],
						game);
				else if (cols == game->col - 1 && (game->map[rows][cols - 1] != '1'))
					draw_block(cols * BPX + game->po[0],
						rows * BPX + game->po[1], game->sprites->tr[3],
						game);
				else if (cols == 0 && (game->map[rows][cols + 1] != '1'))
					draw_block(cols * BPX + game->po[0],
						rows * BPX + game->po[1], game->sprites->tr[5],
						game);
				else if (rows > 0 && game->map[rows][cols] == '1' && 
					game->map[rows - 1][cols] != '1')
					draw_block(cols * BPX + game->po[0],
						rows * BPX + game->po[1], game->sprites->tr[1],
						game);
				else
					draw_block(cols * BPX + game->po[0],
						rows * BPX + game->po[1], game->sprites->tr[4],
						game);
				if (rows != 0 && cols > 0 && game->map[rows][cols - 1] != '1' && game->map[rows][cols] == '1' &&
					game->map[rows][cols + 1] == '1' && game->map[rows - 1][cols] != '1')
					draw_block(cols * BPX + game->po[0],
						rows * BPX + game->po[1], game->sprites->tr[0],
						game);
				else if (rows != 0 && cols < game->col - 1 && game->map[rows][cols + 1] != '1' && game->map[rows][cols] == '1' &&
					game->map[rows][cols - 1] == '1')
					draw_block(cols * BPX + game->po[0],
						rows * BPX + game->po[1], game->sprites->tr[2],
						game);
				else if (cols < game->col - 1 && cols > 0 && 
					game->map[rows][cols + 1] != '1' && game->map[rows][cols - 1] != '1' &&
					game->map[rows][cols] == '1' && game->map[rows - 1][cols] != '1')
					draw_block(cols * BPX + game->po[0],
						rows * BPX + game->po[1], game->sprites->tr[36],
						game);
				else if (cols < game->col - 1 && cols > 0 && 
					game->map[rows][cols + 1] != '1' && game->map[rows][cols - 1] != '1' &&
					game->map[rows][cols] == '1' && game->map[rows - 1][cols] == '1')
					draw_block(cols * BPX + game->po[0],
						rows * BPX + game->po[1], game->sprites->tr[35],
						game);
			}
			else if (game->map[rows][cols] == 'E')
				draw_block(cols * BPX + game->po[0],
				rows * BPX + game->po[1], game->sprites->gt[0],
				game);
		}
	}
}

void	item(game_data *game)
{
	t_sprites	*s;
	int			c;
	int			rows;
	int			cols;

	rows = -1;
	s = game->sprites;
	if (game->coin->current_frame == 9)
		game->coin->current_frame = 0;
	c = (game->coin->current_frame++) % game->coin->max_frame;
	while (++rows <= game->rows)
	{
		cols = -1;
		while (++cols < game->col)
		{
			if (game->map[rows][cols] == 'C' )
			{
				draw_block(cols * BPX + game->po[0],
					rows * BPX + game->po[1], s->c[c],
					game);
			}	
		}
	}
}

void	free_to_all(game_data *game)
{
	free_map(game->map);
	if (game->img != NULL)
	{
		free(game->img);
		game->img = NULL;
	}
	if (game->player != NULL)
	{
		free(game->player);
		game->player = NULL;
	}
	destroy_sprites(game);
}

// General loop of the game which will be executed at best every 15ms
static int	game_loop(game_data *game)
{
	long long	now;
	long long	diff_millisecs;

	now = millitimestamp();
	diff_millisecs = now - game->lm;
	if (diff_millisecs > 15)
	{
		fps(game);
		mlx_clear_window(game->mlx, game->mlx_win);
		player_position(game);
		background(game);
		wall(game);
		item(game);
		//gate(game);
		//monster(game);
		player(game);
		if (game->dbg)
			show_debug(game);
		show_hud(game);
	}
	return (1);
}

int main(int argc, char **argv)
{
	game_data game;
	t_data *img;
	
	if (argc != 2)
	{
		ft_printf ("Invalid number of arguments\n");
		return (1);
	}
	game.mlx = mlx_init();
	mlx_get_screen_size(game.mlx, &game.width, &game.height);
	game.mlx_win = mlx_new_window(game.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	init_struct(&game);
	init_player(&game);
	init_map(argv[1], &game);
	init_sprites(&game);
	init_camera(&game);
	init_enemies(&game);
	hook_register(&game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_loop(game.mlx);
	exit(EXIT_SUCCESS);

	//mlx_loop_hook(game.mlx, game_loop, &game);
	//mlx_loop(game.mlx);
	
	/* init_window(&game);
	
	// Imprima o texto na tela renderizada
	// mlx_string_put(game.mlx, game.mlx_win, (game.width / 2) / 2, (game.height / 2) / 2, 0xFF0000, "Hello, World!");
	mlx_hook(game.mlx_win, 17, 1L << 17, mlx_loop_end, game.mlx);
	mlx_key_hook(game.mlx_win, quiser, game.mlx);
	//rendering(&game, "./textures/Sky.xpm", 512, 10);
	rendering(&game, "./textures/FieldsTileset.xpm", 0, 0);
	rendering(&game, "./textures/FieldsTileset.xpm", 256, 0);
	rendering(&game, "./textures/FieldsTileset.xpm", 512, 0);
	rendering(&game, "./textures/FieldsTileset.xpm", 768, 0);
	rendering(&game, "./textures/FieldsTileset.xpm", 768, 0);
	rendering(&game, "./textures/FieldsTile_64.xpm", 0, 256);
	//rendering(&game, "./textures/Player/Warrior_1/warrior_1_idle.xpm", 500, 200);
	mlx_loop(game.mlx);
	*/
	// free_map(game.map); !double free!
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

