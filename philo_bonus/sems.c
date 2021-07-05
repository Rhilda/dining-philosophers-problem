#include "phil.h"

int	ft_strcpy(char *dst, char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = 0;
	return (i);
}

sem_t	*ft_sem_open(char *name, int value)
{
	return (sem_open(name, O_CREAT | O_EXCL, 0644, value));
}

char	*ft_sem_name(char *sem_name, char *buffer, int idx)
{
	int	i;

	i = ft_strcpy(buffer, sem_name);
	while (idx > 0)
	{
		buffer[i++] = (idx % 10) + '0';
		idx /= 10;
	}
	buffer[i] = 0;
	return (buffer);
}

void	*monitor(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->mutex);
		if (!philo->is_eating && get_time() > philo->starve_time)
		{
			print_message(philo, PHILO_DIED);
			sem_post(philo->mutex);
			sem_post(philo->game->somebody_dead_m);
			return ((void *)0);
		}
		sem_post(philo->mutex);
		usleep(900);
	}
}
