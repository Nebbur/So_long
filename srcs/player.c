#include "../includes/so_long.h"

void	init_player(game_data *game)
{
	game->player->nl = NBR_LIFE;
	game->player->tl = 50;
	game->player->pp[0] = game->player->player_pos[1] * BPX;
	game->player->pp[1] = game->player->player_pos[0] * BPX - 1;
	game->player->col_collected = 0;
	game->player->col_q = 0;
	game->player->d = 1;
}

void actual_position(game_data *game)
{
	int	trigger;
	int	rows;
	int	cols;
	int	x;
	int	y;

	x = game->exit_x;
	y = game->exit_y;
	rows = -1;
	trigger = 0;

	while (++rows < game->rl && trigger == 0)
	{
		cols = -1;
		while (++cols <= game->cl && trigger == 0)
		{
			if (game->visible_map[x][y] == 'P')
			{
				game->player_out = 1;
				trigger = 1;
				player_going_out(game);
			}

			else if (game->visible_map[rows][cols] == 'P')
			{
				game->player->xy[0] = rows;
				game->player->xy[1] = cols;
				draw_block(cols * BPX + game->po[0],
					rows * BPX + game->po[1], game->sprites->p[0],
					game);
			}
		}
	}
}

void	check_coin_position(game_data *game, int x, int y)
{
	if (game->visible_map[x][y] == 'C')
		game->player->col_collected++;
}

static void move_y(game_data *game)
{
	player_st *p;
	int			x;
	int			y;

	p = game->player;
	x = p->xy[0];
	y = p->xy[1];
	if (p->ac[0] == 1 && game->visible_map[x][y - 1] != '1')
	{
		if (game->visible_map[x][y - 1] == 'E' && p->col_collected == p->col_q)
		{
			game->map[x + game->pil][y + game->pic] = '0';
			game->visible_map[x][y - 1] = 'P';
			game->player->xy[1] = y - 1;
			game->moves++;
		}
		else if (game->visible_map[x][y - 1] == 'E' || game->visible_map[x][y - 1] == '1')
			;
		else
		{
			check_coin_position(game, x, y - 1);
			game->map[x + game->pil][y + game->pic] = '0';
			game->visible_map[x][y - 1] = 'P';

			if (game->trig == 0)
				game->player->xy[1] = y - 1;
			else
				game->triggerY = 1;

			p->player_pos[1] = y - 1;
			game->moves++;
		}
	}
	else if (p->ac[0] == 2)
	{
		if (game->visible_map[x][y + 1] == 'E' && p->col_collected == p->col_q)
		{
			game->map[x + game->pil][y + game->pic] = '0';
			game->visible_map[x][y + 1] = 'P';
			game->player->xy[1] = y + 1;
			game->moves++;
		}
		else if (game->visible_map[x][y + 1] == 'E' || game->visible_map[x][y + 1] == '1')
			;
		else
		{
			ft_printf("y + 1\n");
			check_coin_position(game, x, y + 1);
			game->map[x + game->pil][y + game->pic] = '0';
			game->visible_map[x][y + 1] = 'P';
			if (game->trig == 0)
				game->player->xy[1] = y + 1;
			else
				game->triggerY = 1;
			p->player_pos[1] = y + 1;
			game->moves++;
		}
	}
}

void	player_going_out(game_data *game)
{
/* 	int			i;
	t_sprites	*s;

	s = game->sprites;
	i = 1;

	while (++i <= 7)
	{
		ft_printf("i: %i, exit x: %i, exit y %i\n", i, game->exit_x, game->exit_y);

		if (mlx_clear_window(game->mlx, game->mlx_win) == 1)
			ft_printf("clear window success\n");


		draw_block(game->exit_y * BPX + game->po[0], 
			game->exit_x * BPX + game->po[1], game->sprites->gt[i], game);
		usleep(350000);
	}

	ft_printf("OIEFIOEFHIOBFEuewhbfowf\n\n"); */
	print_issue(game);
}

static void move_x(game_data *game)
{
	player_st *p;
	int			x;
	int			y;

	p = game->player;
	x = p->player_pos[0];
	y = p->player_pos[1];
	if (p->ac[0] == 10)
	{
		if (game->visible_map[x - 1][y] == 'E' && p->col_collected == p->col_q)
		{
			game->map[x + game->pil][y + game->pic] = '0';
			game->visible_map[x - 1][y] = 'P';
			game->player->xy[0] = x - 1;
			game->moves++;
		}
		else if (game->visible_map[x - 1][y] == 'E' || game->visible_map[x - 1][y] == '1')
			;
		else
		{
			check_coin_position(game, x - 1, y);
			game->map[x + game->pil][y + game->pic] = '0';
			game->visible_map[x - 1][y] = 'P';
			game->player->xy[0] = x - 1;
			p->player_pos[0] = x - 1;
			game->moves++;
		}
	}
	else if (p->ac[0] == 4)
	{
		if (game->visible_map[x + 1][y] == 'E' && p->col_collected == p->col_q)
		{
			game->visible_map[x][y] = 'E';
			game->visible_map[x + 1][y] = 'P';
			game->player->xy[0] = x + 1;
			game->moves++;
		}
		else if (!(game->visible_map[x + 1][y] == 'E' || game->visible_map[x + 1][y] == '1'))
		{
			check_coin_position(game, x + 1, y);
			game->map[x + game->pil][y + game->pic] = '0';
			game->visible_map[x + 1][y] = 'P';
			game->player->xy[0] = x + 1;
			p->player_pos[0] = x + 1;
			game->moves++;
		}
	}
}

void	check_camera(game_data *game)
{
	/* player_st	*p;

	p = game->player;
	if (p->player_x > 8)
		p->distance_exceeded = p->player_x - 8;
	else
		p->distance_exceeded = 0; */
}

//main function player
void	player(game_data *game)
{
	if (game->player_out == 0)
		actual_position(game);
	if (game->rows > 11)
		check_camera(game);
	move_y(game);
	move_x(game);
	game->player->ac[0] = 0;
}