#ifndef PHILO_H
# define PHILO_H

#include "../minilibx/mlx.h"

# include "../libft/libft.h"
# include "../minilibx/mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <math.h>
# include <stdlib.h>

typedef struct	s_data 
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct player_stt
{
	int	player_y;
	int	player_x;
	int	col_q;
	int	exit_pos;
}		player_st;


typedef struct	game_struct
{
	int		width;
	int		height;
	char	**map;
	void 	*mlx;
    void 	*mlx_win;
	int		col;
	int		rows;
	int		exit_y;
	int		exit_x;
	player_st	*player;
	t_data	*img;
}			game_data;

int main(int argc, char **argv);
void rendering(game_data *game, char *path, int x, int y);
int check_file(char *map);
void    init_struct(game_data *game);

#endif