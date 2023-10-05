/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raveriss <raveriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:46:58 by raveriss          #+#    #+#             */
/*   Updated: 2023/10/05 18:59:32 by raveriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

/**
 * @function init_mutex
 * @brief Initialise les mutex nécessaires.
 *
 * @param data: Pointeur vers la structure t_data.
 *
 * @description Initialise les mutex pour les fourchettes et
 * les ressources partagées.
 *
 * @return int: Retourne SUCCESS en cas de succès, FAILURE sinon.
 *
 * @errors/edge_effects Si l'initialisation échoue, retourne FAILURE.
 *
 * @examples int result = init_mutex(data_ptr);
 *
 * @dependencies Dépend de pthread_mutex_init.
 *
 * @control_flow 1. Initialiser le mutex des ressources partagées.
 *               2. Initialiser le mutex d'écriture.
 *               3. Initialiser les mutex des fourchettes.
 */
int	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	if (pthread_mutex_init(&data->shared_resource_mutex, NULL))
		return (FAILURE);
	if (pthread_mutex_init(&data->write, NULL))
		return (FAILURE);
	while (i < data->global_rules.n_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

/**
 * @function init_malloc
 * @brief Alloue la mémoire pour les structures nécessaires.
 *
 * @param data: Pointeur vers la structure t_data.
 *
 * @description Alloue la mémoire pour les philosophes, les
 * identifiants de thread et les mutex des fourchettes.
 *
 * @return int: Retourne SUCCESS si réussi, sinon FAILURE.
 *
 * @errors/edge_effects Retourne FAILURE en cas d'échec de malloc.
 *
 * @examples int result = init_malloc(data_ptr);
 *
 * @dependencies Dépend de malloc.
 *
 * @control_flow 1. Allouer mémoire pour les philosophes.
 *               2. Allouer mémoire pour les identifiants de thread.
 *               3. Allouer mémoire pour les mutex des fourchettes.
 */
int	init_malloc(t_data *data)
{
	data->philo = malloc(sizeof(t_philo) * data->global_rules.n_philo);
	if (!data->philo)
		return (FAILURE);
	data->thread_ids = malloc(sizeof(pthread_t) * data->global_rules.n_philo);
	if (!data->thread_ids)
		return (FAILURE);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->global_rules.n_philo);
	if (!data->forks)
		return (FAILURE);
	return (SUCCESS);
}

/**
 * @function init
 * @brief Initialise la structure de données et alloue la mémoire.
 *
 * @param data: Pointeur vers la structure t_data.
 * @param argv: Arguments en ligne de commande.
 *
 * @description Initialise les règles, alloue la mémoire, et met en place
 * les mutex et les philosophes.
 *
 * @return int: Retourne SUCCESS si tout est bien initialisé, sinon FAILURE.
 *
 * @errors/edge_effects Retourne FAILURE si malloc ou init_mutex échoue.
 *
 * @examples int result = init(data_ptr, argv);
 *
 * @dependencies Dépend de ft_atoi, init_malloc, init_mutex.
 *
 * @control_flow 1. Initialiser les règles à partir des arguments.
 *               2. Allouer mémoire et initialiser mutex.
 *               3. Initialiser chaque philosophe.
 */
int	init(t_data *data, char **argv)
{
	int		i;
	t_rules	rules;

	i = -1;
	rules.n_philo = ft_atoi(argv[1]);
	rules.t_die = ft_atoi(argv[2]);
	rules.t_eat = ft_atoi(argv[3]);
	rules.t_sleep = ft_atoi(argv[4]);
	rules.t_think = (rules.t_die - rules.t_eat - rules.t_sleep) * 0.75;
	data->global_rules = rules;
	if (init_malloc(data) || init_mutex(data))
		return (FAILURE);
	while (++i < rules.n_philo)
	{
		data->philo[i].shared_data = data;
		data->philo[i].id = i + 1;
		data->philo[i].last_meal = 0;
		data->philo[i].individual_rules = rules;
		if (argv[5])
			data->philo[i].remaining_meal = ft_atoi(argv[5]);
		else
			data->philo[i].remaining_meal = UNLIMITED_MEALS;
	}
	return (SUCCESS);
}
