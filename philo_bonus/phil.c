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
			sem_wait(game->philosophers[id++].eat_m);
		eat_cnt++;
	}
	print_message(&game->philosophers[0], PHILO_OVER);
	sem_post(game->somebody_dead_m);
	return ((void *)0);
}

int	fork_monitor_thread(t_game *game)
{
	pthread_t	tid;

	if (game->least_eat_num > 0)
	{
		if (pthread_create(&tid, NULL, &monitor_least, (void *)game) != 0)
			return (1);
		pthread_detach(tid);
	}
	return (0);
}

int	routine(t_philo *philo)
{
	pthread_t	tid;

	philo->last_eat_time = get_time();
	philo->starve_time = philo->last_eat_time + philo->game->ttd;
	if (pthread_create(&tid, NULL, &monitor, philo) != 0)
		return (1);
	pthread_detach(tid);
	while (1)
	{
		take_forks(philo);
		eat(philo);
		put_down_forks(philo);
		print_message(philo, PHILO_THINK);
	}
	return (0);
}

int	start(t_game *game)
{
	int		i;

	i = 0;
	game->start = get_time();
	while (i < game->philo_num)
	{
		game->philosophers[i].pid = fork();
		if (game->philosophers[i].pid < 0)
			return (1);
		else if (game->philosophers[i].pid == 0)
			routine(&game->philosophers[i]);
		usleep(100);
		i++;
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_game	game;
	int		i;
	int		ret;

	if (argc < 5 || argc > 6)
		return (ft_error("bad argument\n"));
	ret = read_argv(&game, argc, argv);
	if (ret)
	{
		if (ret != 2)
			clear(&game);
		return (ft_error("init error\n"));
	}
	if (fork_monitor_thread(&game) || start(&game))
	{
		clear(&game);
		return (ft_error("wrong action"));
	}
	sem_wait(game.somebody_dead_m);
	i = 0;
	while (i < game.philo_num)
		kill(game.philosophers[i++].pid, SIGKILL);
	clear(&game);
	return (0);
}
