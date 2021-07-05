#include "phil.h"

int	clear(t_game *game)
{
	int	i;

	usleep(100);
	if (game->fork_m)
	{
		i = 0;
		while (i < game->philo_num)
			pthread_mutex_destroy(&game->fork_m[i++]);
		free(game->fork_m);
	}
	if (game->philosophers)
	{
		i = 0;
		while (i < game->philo_num)
		{
			pthread_mutex_destroy(&game->philosophers[i].mutex);
			pthread_mutex_destroy(&game->philosophers[i++].eat_m);
		}
		free(game->philosophers);
	}
	pthread_mutex_destroy(&game->write_m);
	pthread_mutex_destroy(&game->somebody_dead_m);
	return (1);
}

int	ft_error(char *msg)
{
	ft_putstr_fd(msg, 1);
	return (1);
}
