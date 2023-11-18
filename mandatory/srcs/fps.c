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
	debug_msg = ft_strjoin("FPS: ", fps);
	mlx_string_put(game->mlx, game->mlx_win, 20, 43, 10110000, debug_msg);
	free(fps);
	free(debug_msg);
}

// Calculate FPS
void fps(game_data *game)
{
	long long now;

	now = millitimestamp();
	if (now > game->lm)
	{
		game->fps = 960 / (now - game->lm);
		game->lm = now;
	}
}
