#include "../includes/so_long.h"

void	init_player(game_data *game)
{
	game->player->nl = NBR_LIFE;
	game->player->tl = 50;
	game->player->col_collected = 0;
	game->player->col_q = 0;
}

void actual_position(game_data *game)
{
	int	rows;
	int	cols;
	int	x;
	int	y;

	x = game->exit[0];
	y = game->exit[1];
	rows = -1;
	while (++rows < game->rl)
	{
		cols = -1;
		while (++cols <= game->cl)
		{
			if (game->map[x][y] == 'P')
				player_going_out(game);
			else if (game->visible_map[rows][cols] == 'P')
			{
				game->player->xy[0] = rows + game->pil;
				game->player->xy[1] = cols + game->pic;
				draw_block(cols * BPX + game->po[0],
					rows * BPX + game->po[1], game->sprites->p[0], game);
			}
		}
	}
}

void	check_coin_position(game_data *game, int x, int y)
{
	if (game->visible_map[x][y] == 'C')
		game->player->col_collected++;
}

static void aux_move(int xy[2], int rem_x, int rem_y, int t, game_data *game)
{
	if (t == 1)
		check_coin_position(game, xy[0] + rem_x - game->pil, xy[1] + rem_y - game->pic);
	game->map[xy[0]][xy[1]] = '0';
	game->map[xy[0] + rem_x][xy[1] + rem_y] = 'P';
	if (rem_x != 0)
		game->player->xy[0] = xy[0] + rem_x;
	else
		game->player->xy[1] = xy[1] + rem_y;
	game->moves++;
}

static void move_y(game_data *game)
{
	int			xy[2];

	xy[0] = game->player->xy[0];
	xy[1] = game->player->xy[1];
	if (game->player->ac[0] == 1 && game->visible_map[xy[0] - game->pil][xy[1] - 1 - game->pic] != '1')
	{
		if (game->visible_map[xy[0] - game->pil][xy[1] - 1 - game->pic] == 'E' && 
		game->player->col_collected == game->player->col_q)
			aux_move(xy, 0, (-1), 0, game);
		else if (!(game->visible_map[xy[0] - game->pil][xy[1] - 1 - game->pic] == 'E' || 
		game->visible_map[xy[0] - game->pil][xy[1] - 1 - game->pic] == '1'))
			aux_move(xy, 0, (-1), 1, game);
	}
	else if (game->player->ac[0] == 2)
	{
		if (game->visible_map[xy[0] - game->pil][xy[1] + 1 - game->pic] == 'E' && 
		game->player->col_collected == game->player->col_q)
			aux_move(xy, 0, 1, 0, game);
		else if (!(game->visible_map[xy[0] - game->pil][xy[1] + 1 - game->pic] == 'E' || 
		game->visible_map[xy[0] - game->pil][xy[1] + 1 - game->pic] == '1'))
			aux_move(xy, 0, 1, 1, game);
	}
}

void	player_going_out(game_data *game)
{
	mlx_loop_end(game->mlx);
}

static void move_x(game_data *game)
{
	int			xy[2];

	xy[0] = game->player->xy[0];
	xy[1] = game->player->xy[1];
	if (game->player->ac[0] == 10)
	{
		if (game->visible_map[xy[0] - 1 - game->pil][xy[1] - game->pic] == 'E' && 
		game->player->col_collected == game->player->col_q)
			aux_move(xy, (-1), 0, 0, game);
		else if (!(game->visible_map[xy[0] - 1 - game->pil][xy[1] - game->pic] == 'E' || game->visible_map[xy[0] - 1 - game->pil][xy[1] - game->pic] == '1'))
			aux_move(xy, (-1), 0, 1, game);
	}
	else if (game->player->ac[0] == 4)
	{
		if (game->visible_map[xy[0] + 1 - game->pil][xy[1] - game->pic] == 'E' && 
		game->player->col_collected == game->player->col_q)
			aux_move(xy, 1, 0, 0, game);
		else if (!(game->visible_map[xy[0] + 1 - game->pil][xy[1] - game->pic] == 'E' || game->visible_map[xy[0] + 1 - game->pil][xy[1] - game->pic] == '1'))
			aux_move(xy, 1, 0, 1, game);
	}
}

//main function player
void	player(game_data *game)
{
	actual_position(game);
	move_y(game);
	move_x(game);
	game->player->ac[0] = 0;
}