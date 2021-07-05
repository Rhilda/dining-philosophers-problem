#include "phil.h"

int	ft_malloc(void *target, size_t size)
{
	void	**pt;

	pt = (void **)target;
	*pt = malloc(size);
	if (*pt == 0)
		return (0);
	return (1);
}

int	init_philosophers(t_game *game)
{
	int		i;
	char	buffer[50];

	i = 0;
	while (i < game->philo_num)
	{
		game->philosophers[i].is_eating = 0;
		game->philosophers[i].idx = i;
		game->philosophers[i].eat_cnt = 0;
		game->philosophers[i].game = game;
		ft_sem_name(SEMAPHORE_PHILO, (char *)buffer, i);
		sem_unlink(buffer);
		game->philosophers[i].mutex = ft_sem_open(buffer, 1);
		if (game->philosophers[i].mutex < 0)
			return (1);
		ft_sem_name(SEMAPHORE_EAT, (char *)buffer, i);
		sem_unlink(buffer);
		game->philosophers[i].eat_m = ft_sem_open(buffer, 0);
		if (game->philosophers[i].eat_m < 0)
			return (1);
		i++;
	}
	return (0);
}

int	init_semaphores(t_game *game)
{
	sem_unlink(SEMAPHORE_FORK);
	sem_unlink(SEMAPHORE_WRITE);
	sem_unlink(SEMAPHORE_DEAD);
	sem_unlink(SEMAPHORE_DEADW);
	game->fork_m = ft_sem_open(SEMAPHORE_FORK, game->philo_num);
	if (game->fork_m < 0)
		return (1);
	game->write_m = ft_sem_open(SEMAPHORE_WRITE, 1);
	game->somebody_dead_m = ft_sem_open(SEMAPHORE_DEAD, 0);
	game->dead_write_m = ft_sem_open(SEMAPHORE_DEADW, 1);
	if (game->write_m < 0 || game->somebody_dead_m < 0
		|| game->dead_write_m < 0)
		return (1);
	return (0);
}

int	read_argv(t_game *game, int argc, char **argv)
{
	int	m;
	int	p;

	game->philo_num = ft_atoi(argv[1]);
	if (game->philo_num < 1)
		return (2);
	game->ttd = ft_atoi(argv[2]);
	game->tte = ft_atoi(argv[3]);
	game->tts = ft_atoi(argv[4]);
	if (argc == 6)
		game->least_eat_num = ft_atoi(argv[5]);
	else
		game->least_eat_num = 0;
	game->dead = 0;
	game->fork_m = NULL;
	game->philosophers = NULL;
	p = game->philo_num;
	m = ft_malloc(&game->philosophers, sizeof(*(game->philosophers)) * p);
	if (!m)
		return (1);
	init_philosophers(game);
	return (init_semaphores(game));
}
