#include "../includes/so_long.h"

// Return the distance between the player and the nearest obstacle on the right
int get_prwc(game_data *game)
{
    int dist_coll;
    player_st *p = game->player;
    char **m = game->map;

    if (m[(p->bl[1] + HPX + EHPX) / BPX][(p->bl[0]) / BPX + 1] == '1' ||
        m[(p->br[1]) / BPX][(p->br[0]) / BPX + 1] == '1')
    {
        if (m[(p->bl[1] + HPX) / BPX][(p->bl[0] - HPX) / BPX] == '1' ||
            m[p->br[1] / BPX][(p->br[0] - HPX) / BPX] == '1')
            return -(HPX + 10);
        dist_coll = BPX + (BPX * ((p->bl[0]) / BPX) - p->bl[0]) - 1;
        if (dist_coll > MPX)
            return MPX;
        return dist_coll;
    }
    if (m[(p->bl[1] + HPX + EHPX) / BPX][(p->bl[0] - HPX + EHPX) / BPX] == '1' ||
        m[p->br[1] / BPX][(p->br[0] - HPX + EHPX) / BPX] == '1')
        return -(HPX + EHPX + 5);
    return MPX;
}

// Return the distance between the player and the nearest obstacle on the left
int get_plwc(game_data *game)
{
    int dist_coll;
    player_st *p = game->player;
    char **m = game->map;

    if (m[(p->tll[1] + HPX + EHPX) / BPX][(p->tll[0]) / BPX - 1] == '1' ||
        m[(p->tr[1]) / BPX][(p->tr[0]) / BPX - 1] == '1')
    {
        if (m[(p->tll[1] + HPX) / BPX][(p->tll[0] + HPX) / BPX] == '1' ||
            m[p->tr[1] / BPX][(p->tr[0] + HPX) / BPX] == '1')
            return -(HPX + 10);
        dist_coll = p->tll[0] - (BPX * ((p->tll[0]) / BPX));
        if (dist_coll > MPX)
            return MPX;
        return dist_coll;
    }
    if (m[(p->tll[1] + HPX + EHPX) / BPX][(p->tll[0] + HPX + EHPX) / BPX] == '1' ||
        m[p->tr[1] / BPX][(p->tr[0] + HPX + EHPX) / BPX] == '1')
        return -(HPX + EHPX + 5);
    return MPX;
}

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
	while (++rows <= game->rows && trigger == 0)
	{
		cols = -1;
		while (++cols < game->col && trigger == 0)
		{
			if (game->map[x][y] == 'P')
			{
				ft_printf("E igual a P\n");
				game->player_out = 1;
				trigger = 1;
				player_going_out(game);
			}

			else if (game->map[rows][cols] == 'P')
				draw_block(cols * BPX + game->po[0],
					rows * BPX + game->po[1], game->sprites->p[0],
					game);
		}
	}
}

void	check_coin_position(game_data *game, int x, int y)
{
	if (game->map[x][y] == 'C')
		game->player->col_collected++;
}

static void move_y(game_data *game)
{
	player_st *p;
	int			x;
	int			y;

	p = game->player;
	x = p->player_pos[0];
	y = p->player_pos[1];
	if (p->ac[0] == 1 && game->map[x][y - 1] != '1')
	{
		if (game->map[x][y - 1] == 'E' && p->col_collected == p->col_q)
		{
			game->map[x][y] = '0';
			game->map[x][y - 1] = 'P';
			game->moves++;
		}
		else if (game->map[x][y - 1] == 'E' || game->map[x][y - 1] == '1')
			;
		else
		{
			check_coin_position(game, x, y - 1);
			game->map[x][y] = '0';
			game->map[x][y - 1] = 'P';
			p->player_pos[1] = y - 1;
			game->moves++;
		}
	}
	else if (p->ac[0] == 2)
	{
		if (game->map[x][y + 1] == 'E' && p->col_collected == p->col_q)
		{
			game->map[x][y] = '0';
			game->map[x][y + 1] = 'P';
			game->moves++;
		}
		else if (game->map[x][y + 1] == 'E' || game->map[x][y + 1] == '1')
			;
		else
		{
			check_coin_position(game, x, y + 1);
			game->map[x][y] = '0';
			game->map[x][y + 1] = 'P';
			p->player_pos[1] = y + 1;
			game->moves++;
		}
	}
}

void	player_going_out(game_data *game)
{
	int			i;
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

	ft_printf("OIEFIOEFHIOBFEuewhbfowf\n\n");
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
		if (game->map[x - 1][y] == 'E' && p->col_collected == p->col_q)
		{
			game->map[x][y] = '0';
			game->map[x - 1][y] = 'P';
			game->moves++;
		}
		else if (game->map[x - 1][y] == 'E' || game->map[x - 1][y] == '1')
			;
		else
		{
			check_coin_position(game, x - 1, y);
			game->map[x][y] = '0';
			game->map[x - 1][y] = 'P';
			p->player_pos[0] = x - 1;
			game->moves++;
		}
	}
	else if (p->ac[0] == 4)
	{
		if (game->map[x + 1][y] == 'E' && p->col_collected == p->col_q)
		{
			game->map[x][y] = 'E';
			game->map[x + 1][y] = 'P';
			game->moves++;
		}
		else if (game->map[x + 1][y] == 'E' || game->map[x + 1][y] == '1')
			;
		else
		{
			check_coin_position(game, x + 1, y);
			game->map[x][y] = '0';
			game->map[x + 1][y] = 'P';
			p->player_pos[0] = x + 1;
			game->moves++;
		}
	}
}

//main function player
void	player(game_data *game)
{
	if (game->player_out == 0)
		actual_position(game);
	move_y(game);
	move_x(game);
	game->player->ac[0] = 0;
}