#ifndef PHIL_H
# define PHIL_H

# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>

# define PHILO_EAT		0
# define PHILO_SLEEP	1
# define PHILO_FORK		2
# define PHILO_THINK	3
# define PHILO_DIED		4
# define PHILO_OVER		5

struct	s_game;
typedef struct s_philo
{
	int					idx;
	uint64_t			last_eat_time;
	uint64_t			starve_time;
	int					is_eating;
	int					eat_cnt;
	int					lfork;
	int					rfork;
	struct s_game		*game;
	pthread_mutex_t		mutex;
	pthread_mutex_t		eat_m;
}						t_philo;

typedef struct s_game
{
	int					philo_num;
	uint64_t			ttd;
	uint64_t			tte;
	uint64_t			tts;
	int					least_eat_num;
	int					dead;
	uint64_t			start;
	t_philo				*philosophers;
	pthread_mutex_t		somebody_dead_m;
	pthread_mutex_t		write_m;
	pthread_mutex_t		*fork_m;
}						t_game;

int						ft_strlen(char *str);
int						ft_error(char *msg);
int						ft_atoi(char *str);
void					ft_putstr_fd(char *s, int fd);
int						ft_malloc(void *target, size_t size);
void					init_philosophers(t_game *game);
int						init_mutexes(t_game *game);
int						read_argv(t_game *game, int argc, char **argv);
uint64_t				get_time(void);
void					ft_putnbr_fd(uint64_t n, int fd);
void					print_message(t_philo *philo, int type);
int						clear(t_game *game);
void					take_forks(t_philo *philo);
void					put_down_forks(t_philo *philo);
void					eat(t_philo *philo);

#endif
