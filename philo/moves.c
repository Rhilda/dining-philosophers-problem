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
	pthread_mutex_lock(&philo->game->write_m);
	if (!philo->game->dead)
	{
		ft_putnbr_fd(get_time() - philo->game->start, 1);
		ft_putstr_fd(" ", 1);
		if (type != PHILO_OVER)
			ft_putnbr_fd(philo->idx + 1, 1);
		if (type == PHILO_DIED || type == PHILO_OVER)
			philo->game->dead = 1;
		write(1, get_message(type), ft_strlen(get_message(type)));
	}
	pthread_mutex_unlock(&philo->game->write_m);
}

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->game->fork_m[philo->lfork]);
	print_message(philo, PHILO_FORK);
	pthread_mutex_lock(&philo->game->fork_m[philo->rfork]);
	print_message(philo, PHILO_FORK);
}

void	put_down_forks(t_philo *philo)
{
	print_message(philo, PHILO_SLEEP);
	pthread_mutex_unlock(&philo->game->fork_m[philo->lfork]);
	pthread_mutex_unlock(&philo->game->fork_m[philo->rfork]);
	usleep(philo->game->tts * 1000);
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex);
	philo->is_eating = 1;
	philo->last_eat_time = get_time();
	philo->starve_time = philo->last_eat_time + philo->game->ttd;
	print_message(philo, PHILO_EAT);
	usleep(philo->game->tte * 1000);
	philo->eat_cnt++;
	philo->is_eating = 0;
	pthread_mutex_unlock(&philo->mutex);
	pthread_mutex_unlock(&philo->eat_m);
}
