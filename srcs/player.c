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

static void actual_position(game_data *game)
{
	int	rows;
	int	cols;

	rows = -1;
	while (++rows <= game->rows)
	{
		cols = -1;
		while (++cols < game->col)
		{
			if (game->map[rows][cols] == 'P')
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
	if (p->ac[0] == 1)
	{
		if (game->map[x][y - 1] != '1')
		{
			if (game->map[x][y - 1] == 'E' && p->col_collected == p->col_q)
			{
				game->map[x][y - 1] = 'P';
				free_to_all(game);
				mlx_destroy_window(game->mlx, game->mlx_win);			
				exit(EXIT_SUCCESS);
			}
			else if (game->map[x][y - 1] == 'E' || game->map[x][y - 1] == '1')
				;
			else
			{
				check_coin_position(game, x, y - 1);
				game->map[x][y] = '0';
				game->map[x][y - 1] = 'P';
				p->player_pos[1] = y - 1;
			}
		}
	}
	else if (p->ac[0] == 2)
	{
		if (game->map[x][y + 1] == 'E' && p->col_collected == p->col_q)
		{
			game->map[x][y + 1] = 'P';
			free_to_all(game);
			mlx_destroy_window(game->mlx, game->mlx_win);
			exit(EXIT_SUCCESS);
		}
		else if (game->map[x][y + 1] == 'E' || game->map[x][y + 1] == '1')
			;
		else
		{
			check_coin_position(game, x, y + 1);
			game->map[x][y] = '0';
			game->map[x][y + 1] = 'P';
			p->player_pos[1] = y + 1;
		}
	}
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
			game->map[x - 1][y] = 'P';
			free_to_all(game);
			mlx_destroy_window(game->mlx, game->mlx_win);
			exit(EXIT_SUCCESS);
		}
		else if (game->map[x - 1][y] == 'E' || game->map[x - 1][y] == '1')
			;
		else
		{
			check_coin_position(game, x - 1, y);
			game->map[x][y] = '0';
			game->map[x - 1][y] = 'P';
			p->player_pos[0] = x - 1;
		}
	}
	else if (p->ac[0] == 4)
	{
		if (game->map[x + 1][y] == 'E' && p->col_collected == p->col_q)
		{
			game->map[x + 1][y] = 'P';
			free_to_all(game);
			mlx_destroy_window(game->mlx, game->mlx_win);
			exit(EXIT_SUCCESS);
		}
		else if (game->map[x + 1][y] == 'E' || game->map[x + 1][y] == '1')
			;
		else
		{
			check_coin_position(game, x + 1, y);
			game->map[x][y] = '0';
			game->map[x + 1][y] = 'P';
			p->player_pos[0] = x + 1;
		}
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