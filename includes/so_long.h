#ifndef SO_LONG_H
# define SO_LONG_H

#include "../minilibx/mlx.h"

# include "../libft/libft.h"
# include "../minilibx/mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <math.h>
# include <stdlib.h>
# include <sys/time.h>
# include <fcntl.h>

# define WINDOW_TITLE "so_long"
# define WINDOW_WIDTH 1024
# define WINDOW_HEIGHT 768
# define BPX 64
# define HPX 20
# define MPX 1
# define MCF 10 //MAX COIN FRAM
# define EHPX 10
# define NBR_LIFE 3
# define LETTER_KEY_LEFT	97
# define LETTER_KEY_RIGHT	100
# define LETTER_KEY_UP     	119
# define LETTER_KEY_DOWN	115
# define LETTER_KEY_ACTION	101
# define ARROW_KEY_LEFT	 	65361
# define ARROW_KEY_RIGHT	65363
# define ARROW_KEY_UP		65362
# define ARROW_KEY_DOWN		65364
# define SPACE_BAR			32
# define KEY_ESC			65307
# define HUD_LM 920

typedef struct	s_data 
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

/*
	pp = pixel position [0] = rows (x) | [1] = cols (y)
	tl = tick life
	nl = number life
	d = direction 0 = left | 1 = right
	ac = action code
	tll = top left
	tr = top right
	bl = bottom left
	br = bottom right
*/

typedef struct player_stt
{
	int	player_y;
	int	player_x;
	int	col_q;
	int	col_collected;
	int	exit_pos[2];
	int	player_pos[2]; // 0 rows | 1 col
	int	collectable_pos[2];

	int	pp[2];
	int	tl;
	int	nl;
	int	d;
	int	ac[3];
	int	tll[2];
	int	tr[2];
	int	bl[2];
	int	br[2];
}		player_st;

/*
	p = player sprite
	tr = tile rock sprite
	tl = tile lava sprite
	h = heart sprite
	bg = background sprite
	gt = gate (exit) sprite
*/

typedef struct	s_sprites
{
	void	*p[1];
	void	*tr[37];
	void	*tl[14];
	void	*h[2];
	void	*bg[1];
	void	*gt[1];
	void	*c[10];
}				t_sprites;

typedef struct	s_coin
{
	int	current_frame;
	int	max_frame;
}				t_coin;

/// @brief Enemy structure
/*
	pp : pixel position (pp[0] : x, pp[1] : y)
	init_p : initial position (init_p[0] : x, init_p[1] : y)
	a : top left		(a[0] : x, a[1] : y)
	b : top right		(b[0] : x, b[1] : y)
	c : bottom left 	(c[0] : x, c[1] : y)
	d : bottom right 	(d[0] : x, d[1] : y)
	alive : dead (0) or alive (1)
	move : move(1) or not (0)
	ta : tick attack
	td : tick dead
	direction : direction (0 left, 1 right)
	la : last annimation
	ap : annimation position
	ai : annimation interval
	mi : move interval
*/

typedef struct s_enemy
{
	int	pp[2];
	int	init_p[2];
	int	a[2];
	int	b[2];
	int	c[2];
	int	d[2];
	int	alive;
	int	move;
	int	ta;
	int	td;
	int	direction;
	int	la;
	int	ap;
	int	ai;
	int	mi;
}			t_enemy;

/*
	go = global offset
	po = player offset
	e = quantidade de enemies

	width e rows é igual
	height e col é igual
	tenho de ajustar isso, repeticao de variaveis
*/

typedef struct	game_struct
{
	int		width;
	int		height;
	char	**map;
	void 	*mlx;
    void 	*mlx_win;
	int		col;
	int		rows;
	int		exit_x;
	int		lm;
	int		fps;
	int		go[2];
	int 	po[2];
	int		e;
	int		dbg;

	t_sprites	*sprites;
	player_st	*player;
	t_list		*enemies;
	t_data		*img;
	t_coin		*coin;

}			game_data;

int		main(int argc, char **argv);
void 	rendering(game_data *game, char *path, int x, int y);
int 	check_file(char *map);
void    init_struct(game_data *game);

void	init_window(game_data *game);

void	ft_clearlst(t_list **lst);

//			MAP

int		init_map(char *path, game_data *game);
void	free_map(char **map);

int		close_hook(int keycode, game_data *game);
void	hook_register(game_data *game);
int		keyup_hook(int keycode, game_data *game);
int		keydown_hook(int keycode, game_data *game);

void	set_action(int keycode, game_data *game);
void	unset_action(int keycode, game_data *game);
int		get_action_keycode(int keycode);

void	init_enemies(game_data *game);
void	create_enemy(int row, int col, game_data *game);

void	init_camera(game_data *game);

void	init_player(game_data *game);
void	player_position(game_data *game);
void	player(game_data *game);

void	init_sprites(game_data *game);
void	load_sprite(char *s_name, int s_nbr, int s_type, game_data *game);
char	*path_sprite(char *s_name, int i);
void	destroy_sprites(game_data *game);

void show_fps(game_data *game);
void fps(game_data *game);
long long millitimestamp(void);


void	draw_block(int x, int y, void *sprite, game_data *game);

void	show_debug(game_data *game);

void	free_to_all(game_data *game);

/*
char	**create_map(char *path);
int	check_obstacles_or_not(game_data *game);
int		check_rows_cols(game_data *game);
char **createTempMap(game_data *game);
void freeTempMap(game_data *game, char **tempMap);
void	canCollectAll(char **map_copy, game_data *game, int y, int x);
void	canReachExit(char **map_copy, game_data *game, int y, int x);
int	path_check(char *str, game_data *game); */

//			Get Next Line

int	get_lines(char *map);

#endif