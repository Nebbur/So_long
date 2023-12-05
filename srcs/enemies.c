#include "../includes/so_long.h"

// Create enmies in a linked list
void	create_enemy(int row, int col, int id, game_data *game)
{
	t_enemy	*e;

	e = ft_calloc(sizeof(t_enemy), 1);
	if (e == NULL)
		exit(EXIT_FAILURE);
	e->alive		= 1;
	e->direction	= 1;
	e->move			= 1;
	e->td			= 0;
	e->d			= 1;
	e->cont			= 0;
	e->xy[0]		= row;
	e->xy[1]		= col;
	e->id			= id;
	e->xy_last[0]	= row;
	e->xy_last[1]	= col;
	if (game->enemies == NULL)
		game->enemies = ft_lstnew(e);
	else
		ft_lstadd_front(&game->enemies, ft_lstnew(e));
}

void	init_enemies(game_data *game)
{
	int	rows;
	int	cols;
	int	id;

	id = 0;
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
				if (id == 4)
					id = 0;
				create_enemy(rows, cols, id++, game);
			}
		}
	}
}

int	is_it_safe(int x, int y, game_data *game)
{
	if (x < 1 || y < 1 || x >= game->rows || y >= (game->col - 1))
		return (0);
	if (game->map[x][y] == '0')
		return (1);
}

void	aux_move_enemy(int sig, int new_xy[2], int trig, int id, game_data *game)
{
	int	*new;
	int	cons;

	new = &new_xy[1];
	cons = new_xy[0];
	if (trig == 0)
	{
		new = &new_xy[0];
		cons = new_xy[1];
	}
	if (sig == 0)
		(*new)++;
	else
		(*new)--;
	if (trig == 0 && is_it_safe(*new, cons, game) == 0 ||
		trig == 1 && is_it_safe(cons, *new, game) == 0)
	{
		if (sig == 0)
			(*new)--;
		else
			(*new)++;
		game->enemy->d = id;
	}
}

void move_enemy_1(t_enemy *e, game_data *game)
{
	int	new_xy[2];

	e->cont = 0;
	new_xy[0] = e->xy[0];
	new_xy[1] = e->xy[1];
	if (e->d <= 1)
		aux_move_enemy(0, new_xy, 1, 2, game);
	if (e->d == 2)
		aux_move_enemy(1, new_xy, 0, 3, game);
	if (e->d >= 3 && e->d <= 4)
		aux_move_enemy(1, new_xy, 1, 5, game);
	if (e->d >= 5 && e->d <= 6)
		aux_move_enemy(0, new_xy, 0, 7, game);
	if (e->d >= 7 && e->d <= 8)
		aux_move_enemy(0, new_xy, 1, 9, game);
	if (e->d == 9)
		aux_move_enemy(1, new_xy, 0, 0, game);
	if (e->d == 10)
		e->d = 1;
	e->d++;
	if (is_it_safe(new_xy[0], new_xy[1], game) == 1)
	{
		game->map[e->xy[0]][e->xy[1]] = '0';
		e->xy[0] = new_xy[0];
		e->xy[1] = new_xy[1];
		game->map[e->xy[0]][e->xy[1]] = 'M';
	}
}

void move_enemy_2(t_enemy *e, game_data *game)
{
	int	new_xy[2];

	e->cont = 0;
	new_xy[0] = e->xy[0];
	new_xy[1] = e->xy[1];
	if (e->d <= 1)
		aux_move_enemy(1, new_xy, 1, 2, game);
	if (e->d == 2)
		aux_move_enemy(0, new_xy, 0, 3, game);
	if (e->d >= 3 && e->d <= 4)
		aux_move_enemy(0, new_xy, 1, 5, game);
	if (e->d >= 5 && e->d <= 6)
		aux_move_enemy(1, new_xy, 0, 7, game);
	if (e->d >= 7 && e->d <= 8)
		aux_move_enemy(1, new_xy, 1, 9, game);
	if (e->d == 9)
		aux_move_enemy(0, new_xy, 0, 0, game);
	if (e->d == 10)
		e->d = 1;
	e->d++;
	if (is_it_safe(new_xy[0], new_xy[1], game) == 1)
	{
		game->map[e->xy[0]][e->xy[1]] = '0';
		e->xy[0] = new_xy[0];
		e->xy[1] = new_xy[1];
		game->map[e->xy[0]][e->xy[1]] = 'M';
	}
}

void move_enemy_3(t_enemy *e, game_data *game)
{
	int	new_xy[2];

	e->cont = 0;
	new_xy[0] = e->xy[0];
	new_xy[1] = e->xy[1];
	if (game->map[e->xy[0] - 1][e->xy[1]] != '0')
		e->d = 0;
	else if (game->map[e->xy[0] + 1][e->xy[1]] != '0')
		e->d = 1;
	if (e->d == 1 && is_it_safe(--new_xy[0], new_xy[1], game) == 1)
	{
		game->map[e->xy[0]][e->xy[1]] = '0';
		e->xy[0] = new_xy[0];
		e->xy[1] = new_xy[1];
		game->map[e->xy[0]][e->xy[1]] = 'M';
	}
	else if (e->d == 0 && is_it_safe(++new_xy[0], new_xy[1], game) == 1)
	{
		game->map[e->xy[0]][e->xy[1]] = '0';
		e->xy[0] = new_xy[0];
		e->xy[1] = new_xy[1];
		game->map[e->xy[0]][e->xy[1]] = 'M';
	}
}

void move_enemy_4(t_enemy *e, game_data *game)
{
	int	new_xy[2];

	e->cont = 0;
	new_xy[0] = e->xy[0];
	new_xy[1] = e->xy[1];
	if (game->map[e->xy[0]][e->xy[1] - 1] != '0')
		e->d = 0;
	else if (game->map[e->xy[0]][e->xy[1] + 1] != '0')
		e->d = 1;
	if (e->d == 1 && is_it_safe(new_xy[0], --new_xy[1], game) == 1)
	{
		game->map[e->xy[0]][e->xy[1]] = '0';
		e->xy[0] = new_xy[0];
		e->xy[1] = new_xy[1];
		game->map[e->xy[0]][e->xy[1]] = 'M';
	}
	else if (e->d == 0 && is_it_safe(new_xy[0], ++new_xy[1], game) == 1)
	{
		game->map[e->xy[0]][e->xy[1]] = '0';
		e->xy[0] = new_xy[0];
		e->xy[1] = new_xy[1];
		game->map[e->xy[0]][e->xy[1]] = 'M';
	}
}

void	enemy_collision(t_enemy *enemy, int row, int col, game_data *game)
{
	if (game->map[row + 1][col] == 'P' || game->map[row - 1][col] == 'P')
		game->player->nl--;
	if (((game->map[row + 1][col - 1] == 'P' && game->map[row][col - 1] == '0' ) ||
	(game->map[row + 1][col + 1] == 'P' && game->map[row][col + 1] == '0' )) && game->map[row + 1][col] == '0')
		game->player->nl--;
	if (game->map[row][col - 1] == 'P' || game->map[row][col + 1] == 'P')
		game->player->nl--;
	if (((game->map[row - 1][col - 1] == 'P' && game->map[row][col - 1] == '0' ) ||
	(game->map[row - 1][col + 1] == 'P' && game->map[row][col + 1] == '0' )) && game->map[row - 1][col] == '0')
		game->player->nl--;
}

void	move_enemies(t_enemy *enemy, game_data * game)
{
	enemy->xy_last[0] = enemy->xy[0];
	enemy->xy_last[1] = enemy->xy[1];
	if (enemy->id == 0)
		move_enemy_1(enemy, game); // mover no sentido contrario do relogio
	if (enemy->id == 1)
		move_enemy_2(enemy, game); // mover no sentido do relogio
	if (enemy->id == 2)
		move_enemy_3(enemy, game); // mover verticalmente
	if (enemy->id == 3)
		move_enemy_4(enemy, game); // mover horizontalmente
}

void	enemies(game_data *game)
{
	t_list *enemy_node = game->enemies;

	while (enemy_node)
	{
		game->enemy = (t_enemy *)(enemy_node->content);
		game->enemy->cont++;
		if (game->enemy->xy_last[1] < game->enemy->xy[1])
			draw_block((game->enemy->xy[1] - game->pic) * BPX, (game->enemy->xy[0] 
			- game->pil) * BPX, game->sprites->e[0], game);
		else
			draw_block((game->enemy->xy[1] - game->pic) * BPX, (game->enemy->xy[0] 
			- game->pil) * BPX, game->sprites->e[1], game);
		if (game->enemy->cont >= 30) // determina a velocidade com a qual os inimigos se movem
		{
			enemy_collision(game->enemy, game->enemy->xy[0], game->enemy->xy[1], game);
			move_enemies(game->enemy, game);

		}
		enemy_node = enemy_node->next;
	}
}
