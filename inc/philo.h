/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raveriss <raveriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:46:44 by raveriss          #+#    #+#             */
/*   Updated: 2023/10/05 15:46:47 by raveriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/*  Lib.  */
# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <time.h>
# include <unistd.h>
# include <sys/time.h>
# include <errno.h>
# include <limits.h>

/*  RETURN CODES  */
# define SUCCESS 0
# define FAILURE 1

# define IMIN -2147483648
# define IMAX 2147483647

/*  WHILE  */
# define INFINITY_LOOP 1

/*  DEAD  */
# define NO_ONE_DIED 0
# define SOMEONE_DIED 1

/*  CONTROL AND WHITESPACE CHARACTERES  */
# define TAB_CHAR '\t'
# define CARRIAGE_RETURN '\r'
# define SPACE_CHAR ' '

/*  PHILOSOPHER ID MANIPULATIONS  */
# define PHILO_ID_OFFSET 1

/*  THREADS  */
# define FAILURE_CREAT_THREADS 1
# define FAILURE_JOIN_THREADS 2

/*  INIT  */
# define INIT_SUCCESS 0
# define INIT_FAILURE_RULES -1
# define INIT_FAILURE_DEAD -2
# define INIT_INDEX 0
# define INIT_MEALS_EATEN 0
# define INIT_LAST_MEAL 0

/*  TIME  */
# define US_TO_MS 1000
# define SEC_TO_MS 1000

/*  MEALS  */
# define UNLIMITED_MEALS -1

/*  PHILOSOPHERS STATUS  */
# define ALONE 1
# define PHILO_ID_OFFSET 1

/*  FORK  */
# define FORK_FIRST  1
# define FORK_SECOND 2

/*  LOG  */
# define LOG_SLEEPING "%ld %d is sleeping\n"
# define LOG_THINKING "%ld %d is thinking\n"
# define LOG_DIED "%ld %d died\n"
# define LOG_FORK "%ld %d has taken a fork\n"
# define LOG_EAT "%ld %d is eating\n"

/*  ERR  */
# define ERR_ARGS "Problem with args\n"
# define ERR_ALLOC "Problem with allocations\n"
# define ERR_CREAT_THREADS "Problem creating threads\n"
# define ERR_JOIN_THREADS "Problem joining threads\n"
# define ERR_MUTEX_OPEN "problem closing mutex\n"
# define ERR_TIME_TO_DIE_INCORRECT "time_to_die is incorrect.\n"
# define ERR_TIME_TO_EAT_INCORRECT "time_to_eat is incorrect.\n"
# define ERR_TIME_TO_SLEEP_INCORRECT "time_to_sleep is incorrect.\n"
# define ERR_SUM_OF_PHILO_INCORRECT "sum_of_philo is incorrect.\n"
# define ERR_TIME_TO_DIE_BEFORE "time_to_die before checking: %d\n"
# define ERR_INIT_RULES "Error: Initialization of rules failed.\n"
# define ERR_INIT_DEAD "Error: Initialization of variable 'dead' failed.\n"

/*  TYPEDEF STRUCT  */
struct	s_data;

typedef struct s_rules
{
	int				n_philo;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				t_think;
}				t_rules;

typedef struct s_philo
{
	int				id;
	int				remaining_meal;
	long			last_meal;
	t_rules			individual_rules;
	struct s_data	*shared_data;
}				t_philo;

typedef struct s_data
{
	long long		t0;
	int				someone_died;
	pthread_mutex_t	*forks;
	t_philo			*philo;
	pthread_t		*thread_ids;
	pthread_mutex_t	shared_resource_mutex;
	pthread_mutex_t	write;
	t_rules			global_rules;
}				t_data;

/*  PROTOTYPE  */
void	*run_philosopher_routine(void *valise);
long	get_time(long t0);
int		init(t_data *data, char **argv);
void	check_philosopher_life(t_data *data);
int		ft_atoi(const char *nptr);
void	printf_meal(t_philo *philo);
void	ft_usleep(t_data *data, long duration);
void	print_meal(t_philo *philo);
int		should_philo_continue(t_data *data, t_philo *philo);
int		init_philosopher_rules(t_data *data, t_rules rules);
void	*ft_memset(void *s, int c, size_t n);
int		cleanup_and_end(t_data *data);
long	ft_atol(const char *nptr);

#endif