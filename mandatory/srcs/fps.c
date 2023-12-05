#include "../includes/so_long.h"

// Get timestamp in milliseconds
long long millitimestamp(void)
{
	struct timeval timeval;
	long long microtime;

	gettimeofday(&timeval, NULL);
	microtime = timeval.tv_sec * 1000LL + timeval.tv_usec / 1000;
	return (microtime);
}

// Show FPS in the window
void show_fps(game_data *game)
{
	char *debug_msg;
	char *fps;

	fps = ft_itoa(game->fps);
	ft_printf("\nFPS %i %i\n", fps, game->fps);
	debug_msg = ft_strjoin("FPS: ", fps);
	ft_printf("debug %i\n", debug_msg);
	mlx_string_put(game->mlx, game->mlx_win, 20, 43, 10110000, debug_msg);
	free(fps);
	free(debug_msg);
}

/* // Calculate FPS
void fps(game_data *game)
{
	long long now;

	now = millitimestamp();
	//ft_printf("game lm %i\n", game->lm);
	if (now > game->lm)
	{
		game->fps = 960 / (now - game->lm);
		game->lm = now;
	}
} */

void fps(game_data *game)
{
    game->fps = game->frame_count / (game->elapsed_time / 1000000.0); // Convertendo para segundos
	game->frame_count = 0;
    game->last_time = game->current_time;
}