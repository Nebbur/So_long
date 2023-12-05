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
# define MCF 10 //MAX COIN FRAME
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
	int	init_xy[2];
	int	xy[2];
	int	xy_next[2];
	int xy_last[2];
	int xy_sto_last[2]; //second to last
	int	col_q;
	int	col_collected;
	int	exit_pos[2];
	int	player_pos[2]; // 0 rows | 1 col
	int	collectable_pos[2];

	int	cont;

	int	distance_exceeded;

	int	tl;
	int	nl;
	int	d;
	int	ac[3];
}		player_st;

/*
	p	= player 		sprite
	tr	= tile rock 	sprite
	tl	= tile lava 	sprite
	h	= heart 		sprite
	bg	= background 	sprite
	gt	= gate (exit) 	sprite
	s	= scroll 		sprite
	l	= letter 		sprite
*/

typedef struct	s_sprites
{
	void	*p[1];
	void	*tr[37];
	void	*tl[14];
	void	*h[2];
	void	*bg[1];
	void	*gt[7];
	void	*c[10];
	void	*s[4];
	void	*l[4];
	void	*e[2];
}				t_sprites;

typedef struct	s_coin
{
	int	current_frame;
	int	max_frame;
}				t_coin;

/// @brief Enemy structure
/*
	alive : dead (0) or alive (1)
	move : move(1) or not (0)
	ta : tick attack
	td : tick dead
	direction : direction (0 left, 1 right)
	mi : move interval
*/

typedef struct s_enemy
{
	long long	last_move_time;  // Nova variável para armazenar o tempo do último movimento
	int			alive;
	int			move;
	int			ta;
	int			td;
	int			direction;
	int			mi;
	int			xy[2];
	int			xy_last[2];
	int			d;
	int			id;
	int			cont;
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
	long long	frame_count;
	long long	last_time;
	long long	elapsed_time;
	long long	current_time;

	int		cont; //					-- enemies

	int		is; //initial steps			-- enemies
	int		id; // initial direction	-- enemies

	int		width;
	int		height;
	char	**map;
	char	**visible_map;
	
	int		rl; // row limit
	int		cl; // col limit
	int		trig;

	void 	*mlx;
    void 	*mlx_win;
	int		col;
	int		rows;
	int		exit[2];
	int		lm;
	int		fps;
	int		go[2];
	int		e;
	int		dbg;
	int		moves;
	int		player_out;
	int		pic;
	int		pil;
	int		triggerX;
	int		triggerY;
	int		triggerY_temp;
	int		triggerY_temp_2;
	int		sinal;

	t_sprites	*sprites;
	player_st	*player;
	t_enemy		*enemy;		//enemy node
	t_list		*enemies;	//enemies list
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
void	create_enemy(int row, int col, int id, game_data *game);
void	enemies(game_data *game);

void	init_camera(game_data *game);

void	player_position_onthemap(game_data *game);
void	init_player(game_data *game);
void	player_position(game_data *game);
void	player(game_data *game);
void	player_going_out(game_data *game);
void	actual_position(game_data *game);

void	init_sprites(game_data *game);
void	load_sprite(char *s_name, int s_nbr, int s_type, game_data *game);
char	*path_sprite(char *s_name, int i);
void	destroy_sprites(game_data *game);

void	show_fps(game_data *game);
void	fps(game_data *game);
long long millitimestamp(void);


void	draw_block(int x, int y, void *sprite, game_data *game);

void	show_debug(game_data *game);

void	free_to_all(game_data *game);

void	print_issue(game_data *game);

void 	init_visible_map(game_data *game);
void	process_map(game_data *game);

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