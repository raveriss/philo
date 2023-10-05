/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raveriss <raveriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 11:41:31 by raveriss          #+#    #+#             */
/*   Updated: 2023/10/05 18:54:37 by raveriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

/**
 * @function free_forks_p_philo
 * @brief Libère les ressources associées aux philosophes.
 *
 * @param data: Pointeur vers la structure t_data.
 *
 * @description Libère la mémoire allouée pour les fourchettes,
 * les IDs des threads, et les philosophes.
 *
 * @return void
 * 
 * @errors/edge_effects Peut entraîner une fuite de mémoire si mal
 * appelée. Assurez-vous que les ressources sont initialisées.
 *
 * @examples free_forks_p_philo(data_ptr);
 *
 * @dependencies Dépend des fonctions free standard.
 *
 * @control_flow 1. Vérifier chaque ressource.
 *               2. Libérer la mémoire si nécessaire.
 */
void	free_forks_p_philo(t_data *data)
{
	if (data->forks)
		free(data->forks);
	if (data->thread_ids)
		free(data->thread_ids);
	if (data->philo)
		free(data->philo);
}

/**
 * @function cleanup_and_end
 * @brief Nettoie les ressources et termine le programme.
 *
 * @param data: Pointeur vers la structure t_data.
 *
 * @description Détruit les mutex pour les fourchettes et les
 * ressources partagées, puis libère la mémoire allouée.
 *
 * @return int: Retourne SUCCESS en cas de succès, FAILURE sinon.
 *
 * @errors/edge_effects En cas d'échec de la destruction des mutex,
 * retourne FAILURE.
 *
 * @examples int result = cleanup_and_end(data_ptr);
 *
 * @dependencies Dépend de pthread_mutex_destroy et de
 * free_forks_p_philo.
 *
 * @control_flow 1. Détruire les mutex des fourchettes.
 *               2. Détruire les autres mutex.
 *               3. Appeler free_forks_p_philo.
 */
int	cleanup_and_end(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->global_rules.n_philo)
	{
		if (pthread_mutex_destroy(&data->forks[i]))
			return (FAILURE);
		i++;
	}
	if (pthread_mutex_destroy(&data->shared_resource_mutex))
		return (FAILURE);
	if (pthread_mutex_destroy(&data->write))
		return (FAILURE);
	free_forks_p_philo(data);
	return (SUCCESS);
}
