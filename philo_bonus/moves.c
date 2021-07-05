#include "phil.h"

char	*get_message(int type)
{
	if (type == PHILO_EAT)
		return (" is eating\n");
	else if (type == PHILO_SLEEP)
		return (" is sleeping\n");
	else if (type == PHILO_FORK)
		return (" takes a fork\n");
	else if (type == PHILO_THINK)
		return (" is thinking\n");
	else if (type == PHILO_OVER)
		return ("least eat count reached\n");
	else
		return (" died\n");
}

void	print_message(t_philo *philo, int type)
{
	sem_wait(philo->game->write_m);
	sem_wait(philo->game->dead_write_m);
	ft_putnbr_fd(get_time() - philo->game->start, 1);
	ft_putstr_fd(" ", 1);
	if (type != PHILO_OVER)
		ft_putnbr_fd(philo->idx + 1, 1);
	write(1, get_message(type), ft_strlen(get_message(type)));
	if (!(type == PHILO_DIED || type == PHILO_OVER))
		sem_post(philo->game->dead_write_m);
	sem_post(philo->game->write_m);
}

void	take_forks(t_philo *philo)
{
	sem_wait(philo->game->fork_m);
	print_message(philo, PHILO_FORK);
	sem_wait(philo->game->fork_m);
	print_message(philo, PHILO_FORK);
}

void	put_down_forks(t_philo *philo)
{
	print_message(philo, PHILO_SLEEP);
	sem_post(philo->game->fork_m);
	sem_post(philo->game->fork_m);
	usleep(philo->game->tts * 1000);
}

void	eat(t_philo *philo)
{
	sem_wait(philo->mutex);
	philo->is_eating = 1;
	philo->last_eat_time = get_time();
	philo->starve_time = philo->last_eat_time + philo->game->ttd;
	print_message(philo, PHILO_EAT);
	usleep(philo->game->tte * 1000);
	philo->eat_cnt++;
	philo->is_eating = 0;
	sem_post(philo->mutex);
	sem_post(philo->eat_m);
}
