#include "../includes/so_long.h"

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
	game->enemies = NULL;
	/* game->enemies = calloc(sizeof(t_list), 1);
	if (game->enemies == NULL)
	{
		free(game->enemies);
		exit(EXIT_FAILURE);
	} */
	game->frame_count = 0;
	game->last_time = 0;
	game->elapsed_time = 0;

	game->sinal = 1;
	game->trig = 0;
	game->triggerY_temp = 0;
	game->triggerY_temp_2 = 0;
	game->triggerY = 0;
	game->triggerX = 0;
	game->player_out = 0;
	game->map = NULL;
	game->visible_map = NULL;
	game->img->img = NULL;
	game->img->bits_per_pixel = 2000;
	game->img->line_length = 200;
	game->img->endian = 1500;
	game->po[0] = 0;
	game->po[1] = 0;
	game->lm = 0;
	game->moves = 0;
	game->sprites = ft_calloc(sizeof(t_sprites), 1);
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

	game->width = 0;
	game->height = 0;
	game->col = 0;
	game->rows = 0;
	game->exit[0] = 0;
	game->exit[1] = 0;

	game->player->cont = 0;
	game->player->xy_next[1] = 0;
	game->player->xy_sto_last[0] = -1;
	game->player->xy_sto_last[1] = -1;
	game->player->init_xy[0] = 0;
	game->player->init_xy[1] = 0;
	game->player->player_pos[0] = 0;
	game->player->player_pos[1] = 0;
	game->player->col_collected = 0;
	game->player->col_q = 0;
	game->player->collectable_pos[0] = 0;
	game->player->collectable_pos[1] = 0;
	game->player->distance_exceeded = 0;
	game->dbg = 0;
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
		else if (s_type == 8)
			game->sprites->s[i] = mlx_xpm_file_to_image(game->mlx, path, &d, &d);
		else if (s_type == 9)
			game->sprites->l[i] = mlx_xpm_file_to_image(game->mlx, path, &d, &d);
		else if (s_type == 10)
			game->sprites->e[i] = mlx_xpm_file_to_image(game->mlx, path, &d, &d);
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
		else if (s_type == 8)
			mlx_destroy_image(game->mlx, game->sprites->s[i]);
		else if (s_type == 9)
			mlx_destroy_image(game->mlx, game->sprites->l[i]);
		else if (s_type == 10)
			mlx_destroy_image(game->mlx, game->sprites->e[i]);
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
	destroy_sprite("gate", 7, 6, game);
	destroy_sprite("coin", 10, 7, game);
	destroy_sprite("scroll", 2, 8, game);
	destroy_sprite("letter", 4, 9, game);
	destroy_sprite("enemy", 1, 10, game);
	free(game->sprites);
}

void	init_sprites(game_data *game)
{
	load_sprite("player", 1, 1, game);
	load_sprite("tile_rock", 37, 2, game);
	load_sprite("lava_tile", 14, 3, game);
	load_sprite("heart", 2, 4, game);
	load_sprite("background", 1, 5, game);
	load_sprite("gate", 7, 6, game);
	load_sprite("coin", 10, 7, game);
	load_sprite("scroll", 2, 8, game);
	load_sprite("letter", 4, 9, game);
	load_sprite("enemy", 1, 10, game);
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
	e->alive = 1;
	e->direction = 1;
	e->move = 1;
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
		mlx_loop_end(game->mlx);
	return (0);
}

void	hook_register(game_data *game)
{
	mlx_hook(game->mlx_win, 17, 1L << 0, close_hook, game);
	mlx_hook(game->mlx_win, 2, 1L << 0, keydown_hook, game);
	mlx_hook(game->mlx_win, 3, 1L << 1, keyup_hook, game);
}

// Função para converter decimal para binário e retornar como inteiro
int decimalParaBinario(int decimal)
{
	int binario = 0, base = 1;
	int resto;

	if (decimal == 0)
		return (0);

	while (decimal > 0)
	{
		resto = decimal % 2;
		binario = binario + resto * base;
		decimal = decimal / 2;
		base = base * 10;
	}

	return (binario);
}

void	show_moves(game_data *game)
{
	char	*m;
	char	*debug_msg;
	int		color;

	color = 00110010 + decimalParaBinario(game->moves);

	m = ft_itoa(game->moves);
	debug_msg = ft_strjoin("MOVES: ", m);
	free (m);
	mlx_string_put(game->mlx, game->mlx_win, 20, 24, color, debug_msg);
	free (debug_msg);
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
	{
		//ft_printf("%i, %i\n", x, y);
		//ft_printf("nao printa\n");
		return ;
	}
	mlx_put_image_to_window(game->mlx, game->mlx_win,
		sprite, x, y );
}

static void	background(game_data *game)
{
	int	rows;
	int	cols;

	rows = -1;
	while (++rows < game->rl)
	{
		cols = -1;
		while (++cols < game->cl)
		{
			if (game->visible_map[rows][cols] == '0')
			{
				draw_block(cols * BPX + game->po[0],
					rows * BPX + game->po[1], game->sprites->bg[0],
					game);
			}
		}
	}
}

void	player_position(game_data *game)
{
	player_st	*p;

	p = game->player;
	if (p->xy_last[0] != p->xy[0])
		p->xy_last[0] = p->xy[0];
	if (p->xy_last[1] != p->xy[1])
		p->xy_last[1] = p->xy[1];
}

void	show_hud(game_data *game)
{
	show_moves(game);
	//mlx_put_image_to_window(game->mlx, game->mlx_win, game->sprites->h[0], \
	//HUD_LM + 40, 40);
	//show_movement(so_long);
	//show_life(so_long);
}

void	wall(game_data *game)
{
	int	r;
	int	c;
	char **m;
	int	trigger;

	trigger = 1;
	m = game->visible_map;
	r = -1;
	while (m[++r])
	{
		c = -1;
		while (m[r][++c])
		{
			if (m[r][c] == '1')
			{
				if (r != 0 && c > 0 && m[r - 1][c] == '1' &&
					m[r][c - 1] == '1' && m[r - 1][c - 1] != '1')
					draw_block(c * BPX + game->po[0],
						r * BPX + game->po[1], game->sprites->tr[12],
						game);
				else if (r != 0 && m[r - 1][c] == '1' &&
					m[r][c + 1] == '1' && m[r - 1][c + 1] != '1')
					draw_block(c * BPX + game->po[0],
						r * BPX + game->po[1], game->sprites->tr[11],
						game);
				else if (c == game->col - 1 && (m[r][c - 1] != '1'))
					draw_block(c * BPX + game->po[0],
						r * BPX + game->po[1], game->sprites->tr[3],
						game);
				else if (c == 0 && (m[r][c + 1] != '1'))
					draw_block(c * BPX + game->po[0],
						r * BPX + game->po[1], game->sprites->tr[5],
						game);
				else if (r > 0 && m[r][c] == '1' && 
					m[r - 1][c] != '1')
					draw_block(c * BPX + game->po[0],
						r * BPX + game->po[1], game->sprites->tr[1],
						game);
				else
				{
					if (c == 0 && r == 0)
						draw_block(c * BPX + game->po[0],
							r * BPX + game->po[1], game->sprites->s[0],
							game);
					else if (c == 1 && r == 0)
						draw_block(c * BPX + game->po[0],
							r * BPX + game->po[1], game->sprites->s[1],
							game);
					else
						draw_block(c * BPX + game->po[0],
							r * BPX + game->po[1], game->sprites->tr[4],
							game);
					trigger = 0;
				}
				if (trigger == 1)
				{
					if (r != 0 && c > 0 && m[r][c - 1] != '1' && m[r][c] == '1' &&
						m[r][c + 1] == '1' && m[r - 1][c] != '1')
						draw_block(c * BPX + game->po[0],
							r * BPX + game->po[1], game->sprites->tr[0],
							game);
					else if (r != 0 && c < game->col - 1 && c > 0 && m[r][c + 1] != '1' && m[r][c] == '1' &&
						m[r][c - 1] == '1')
						draw_block(c * BPX + game->po[0],
							r * BPX + game->po[1], game->sprites->tr[2],
							game);
					else if (c < game->col - 1 && c > 0 && 
						m[r][c + 1] != '1' && m[r][c - 1] != '1' &&
						m[r][c] == '1' && m[r - 1][c] != '1')
						draw_block(c * BPX + game->po[0],
							r * BPX + game->po[1], game->sprites->tr[36],
							game);
					else if (c < game->col - 1 && c > 0 && 
						m[r][c + 1] != '1' && m[r][c - 1] != '1' &&
						m[r][c] == '1' && m[r - 1][c] == '1')
						draw_block(c * BPX + game->po[0],
							r * BPX + game->po[1], game->sprites->tr[35],
							game);
				}
				trigger = 1;
			}
			else if (c < game->col && m[r][c] == 'E')
				draw_block(c * BPX + game->po[0],
				r * BPX + game->po[1], game->sprites->gt[0],
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
	while (++rows < game->rl)
	{
		cols = -1;
		while (++cols <= game->cl)
		{
			if (game->visible_map[rows][cols] == 'C' )
			{
				draw_block(cols * BPX + game->po[0],
					rows * BPX + game->po[1], s->c[c],
					game);
			}
		}
	}
}

void ft_clearlst(t_list **lst)
{
	t_list *temp;

	temp = *lst;
	while (temp != NULL)
	{
		t_list *next = temp->next;
		free(temp);
		temp = next;
	}

	*lst = NULL;
}


void	free_to_all(game_data *game)
{
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
	if (game->enemies != NULL)
	{
		ft_clearlst(&game->enemies);
		game->enemies = NULL;
	}
	free(game->coin);
	destroy_sprites(game);
	mlx_destroy_window(game->mlx, game->mlx_win);
	mlx_destroy_display(game->mlx);
	free(game->mlx);
	free_map(game->map);
	free_map(game->visible_map);
}

// General loop of the game which will be executed at best every 15ms
static int	game_loop(game_data *game)
{
	long long	now;
	long long	diff_millisecs;
	int			trigger;

	trigger = 0;
    game->current_time = millitimestamp();
    game->elapsed_time = (game->current_time - game->last_time) * 1000;
	game->frame_count++;
	if (game->elapsed_time >= 1000000 / 30)
	{
		fps(game);
		if (mlx_clear_window(game->mlx, game->mlx_win) == 1)
			ft_printf("clear window success\n");
		background(game);
		wall(game);
		item(game);
		//monster(game);
		player(game);
		show_debug(game);
		process_map(game);
		player_position_onthemap(game);
		player_position(game);
		show_hud(game);
		ft_printf("\n\nCOIN COLLECTED::::::::::::::::::::::::::::::::::::::::: %i\nCOIN TO COLLECT:::::::::::::::::::::::::::::::::::::::: %i\n", game->player->col_collected, game->player->col_q);
		return (1);
	}
}

int main(int argc, char **argv)
{
	game_data game;

	if (argc != 2)
	{
		ft_printf ("Invalid number of arguments\n");
		return (1);
	}
	game.mlx = mlx_init();
	//verify_alloc(game.mlx);
	if (!game.mlx)
		mlx_destroy_display(game.mlx);
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
	free_to_all(&game);
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

