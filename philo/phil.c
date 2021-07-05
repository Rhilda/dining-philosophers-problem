#include "phil.h"

void	*monitor_least(void *arg)
{
	t_game	*game;
	int		id;
	int		eat_cnt;

	game = (t_game *)arg;
	eat_cnt = 0;
	while (eat_cnt < game->least_eat_num)
	{
		id = 0;
		while (id < game->philo_num)
			pthread_mutex_lock(&game->philosophers[id++].eat_m);
		eat_cnt++;
	}
	print_message(&game->philosophers[0], PHILO_OVER);
	pthread_mutex_unlock(&game->somebody_dead_m);
	return ((void *)0);
}

void	*monitor(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&philo->mutex);
		if (!philo->is_eating && get_time() > philo->starve_time)
		{
			print_message(philo, PHILO_DIED);
			pthread_mutex_unlock(&philo->mutex);
			pthread_mutex_unlock(&philo->game->somebody_dead_m);
			return ((void *)0);
		}
		pthread_mutex_unlock(&philo->mutex);
		usleep(900);
	}
}

void	*routine(void *arg)
{
	t_philo		*philo;
	pthread_t	tid;

	philo = (t_philo *)arg;
	philo->last_eat_time = get_time();
	philo->starve_time = philo->last_eat_time + philo->game->ttd;
	if (pthread_create(&tid, NULL, &monitor, arg) != 0)
		return ((void *)1);
	pthread_detach(tid);
	while (1)
	{
		take_forks(philo);
		eat(philo);
		put_down_forks(philo);
		print_message(philo, PHILO_THINK);
	}
	return ((void *)0);
}

int	start(t_game *game)
{
	pthread_t	tid;
	int			i;

	i = 0;
	game->start = get_time();
	if (game->least_eat_num > 0)
	{
		if (pthread_create(&tid, NULL, &monitor_least, (void *)game) != 0)
			return (1);
		pthread_detach(tid);
	}
	while (i < game->philo_num)
	{
		if ((pthread_create(&tid, NULL, &routine,
					(void *)(&game->philosophers[i]))) != 0)
			return (1);
		pthread_detach(tid);
		usleep(100);
		i++;
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_game	game;
	int		ret;

	if (argc < 5 || argc > 6)
		return (ft_error("bad argument\n"));
	ret = read_argv(&game, argc, argv);
	if ((ret))
	{
		if (ret != 2)
			clear(&game);
		return (ft_error("init error\n"));
	}
	if (start(&game))
	{
		clear(&game);
		return (ft_error("wrong action"));
	}
	pthread_mutex_lock(&game.somebody_dead_m);
	pthread_mutex_unlock(&game.somebody_dead_m);
	clear(&game);
	return (0);
}
