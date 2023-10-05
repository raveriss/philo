/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_death.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raveriss <raveriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 10:37:33 by raveriss          #+#    #+#             */
/*   Updated: 2023/10/05 18:54:42 by raveriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

/**
 * @function signal_and_print_death
 * @brief Signale la mort d'un philosophe et imprime le message.
 *
 * @param data: Pointeur vers la structure t_data.
 * @param i: L'index du philosophe.
 * 
 * @description Cette fonction met à jour le flag 'someone_died' 
 * et imprime un message indiquant quel philosophe est mort.
 *
 * @return void
 * 
 * @errors/edge_effects La fonction prend les verrous pour éviter les
 * conflits. Ceci peut causer un blocage si mal utilisé.
 *
 * @examples signal_and_print_death(data_ptr, 2);
 *
 * @dependencies Nécessite l'accès à pthread_mutex_lock,
 * pthread_mutex_unlock et printf.
 *
 * @control_flow 1. Prendre les mutex.
 *               2. Mettre à jour les données.
 *               3. Libérer les mutex.
 */
void	signal_and_print_death(t_data *data, int i)
{
	pthread_mutex_lock(&data->shared_resource_mutex);
	data->someone_died = SOMEONE_DIED;
	pthread_mutex_unlock(&data->shared_resource_mutex);
	pthread_mutex_lock(&data->write);
	printf(LOG_DIED, get_time(data->t0), i);
	pthread_mutex_unlock(&data->write);
}

/**
 * @function update_philosopher_data
 * @brief Met à jour les données d'un philosophe.
 *
 * @param data: Pointeur vers la structure t_data.
 * @param iteration_meal_count: Pointeur vers le compteur de repas.
 * @param time_since_last_meal: Pointeur vers le temps depuis le dernier repas.
 * @param i: L'index du philosophe.
 * 
 * @description Met à jour les données associées à un philosophe.
 * Ceci inclut le nombre de repas restants et le temps écoulé
 * depuis le dernier repas.
 *
 * @return void
 * 
 * @errors/edge_effects Utilise les verrous pour prévenir les conflits.
 *
 * @examples update_philosopher_data(data_ptr, &count, &time, 2);
 *
 * @dependencies Nécessite l'accès à pthread_mutex_lock,
 * pthread_mutex_unlock et get_time.
 *
 * @control_flow 1. Prendre le mutex.
 *               2. Mettre à jour les données.
 *               3. Libérer le mutex.
 */
void	update_philosopher_data(t_data *data, int *iteration_meal_count,
	int *time_since_last_meal, int i)
{
	pthread_mutex_lock(&data->shared_resource_mutex);
	*iteration_meal_count += data->philo[i].remaining_meal;
	*time_since_last_meal = get_time(data->t0)
		- data->philo[i].last_meal;
	pthread_mutex_unlock(&data->shared_resource_mutex);
}

/**
 * @function check_philosopher_life
 * @brief Vérifie l'état de vie des philosophes.
 *
 * @param data: Pointeur vers la structure t_data.
 * 
 * @description Parcourt chaque philosophe pour vérifier son
 * état. Si un philosophe doit mourir, la fonction signale
 * et imprime la mort.
 *
 * @return void
 * 
 * @errors/edge_effects Utilise les verrous et peut appeler 
 * signal_and_print_death. 
 * 
 * @examples check_philosopher_life(data_ptr);
 *
 * @dependencies Dépend de update_philosopher_data et 
 * signal_and_print_death.
 *
 * @control_flow 1. Initialiser les variables.
 *               2. Boucle infinie pour vérifier chaque philosophe.
 *               3. Appeler update_philosopher_data.
 *               4. Vérifier les conditions et agir en conséquence.
 */
void	check_philosopher_life(t_data *data)
{
	int	i;
	int	time_since_last_meal;
	int	iteration_meal_count;

	i = INIT_INDEX;
	iteration_meal_count = INIT_MEALS_EATEN;
	while (INFINITY_LOOP)
	{
		update_philosopher_data(data, &iteration_meal_count,
			&time_since_last_meal, i);
		if (time_since_last_meal > data->global_rules.t_die
			&& data->philo[i].remaining_meal)
		{
			signal_and_print_death(data, i + PHILO_ID_OFFSET);
			return ;
		}
		if (i == data->global_rules.n_philo - 1
			&& !iteration_meal_count)
			return ;
		else if (i == data->global_rules.n_philo - 1
			&& iteration_meal_count)
			iteration_meal_count = INIT_MEALS_EATEN;
		i++;
		i = i % data->global_rules.n_philo;
	}
}
