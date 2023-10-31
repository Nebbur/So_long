#ifndef CHECKER_H
# define CHECKER_H

# include <math.h>
# include <sys/time.h>
# include <stddef.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <stdarg.h>
# include <limits.h>
# define BUFFER_SIZE 1024

typedef struct player_stt
{
	int	player_y;
	int	player_x;
	int	col_q;
	int	col_collected;
	int	exit_pos[2];
	int	player_pos[2]; // 0 rows | 1 col
	int	collectable_pos[2];
}		player_st;

typedef struct	game_struct
{
	int	    	width;
	int	    	height;
	char    	**map;
	int	    	col;
	int	    	rows;
	int	    	exit_x;
	int	    	e;
    player_st *player;
}			game_data;

char	*get_next_line(int fd);


#endif