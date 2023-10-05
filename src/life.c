/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raveriss <raveriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:47:07 by raveriss          #+#    #+#             */
/*   Updated: 2023/10/05 19:05:49 by raveriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

/**
 * @function consume_meal_and_update_time
 * @brief Gère la consommation d'un repas par un philosophe.
 *
 * @param philo: Pointeur vers la structure t_philo.
 *
 * @description Mise à jour de l'heure du dernier repas et impression
 * du statut. Réduit le nombre de repas restants.
 *
 * @return void
 *
 * @errors/edge_effects Utilise des mutex, peut bloquer.
 *
 * @examples consume_meal_and_update_time(philo_ptr);
 *
 * @dependencies Dépend de get_time, print_meal, ft_usleep.
 *
 * @control_flow 1. Verrouiller shared_resource_mutex.
 *               2. Mise à jour last_meal.
 *               3. Déverrouiller et impression.
 *               4. Attendre pour manger.
 *               5. Réduire remaining_meal.
 */
void	consume_meal_and_update_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->shared_data->shared_resource_mutex);
	philo->last_meal = get_time(philo->shared_data->t0);
	pthread_mutex_unlock(&philo->shared_data->shared_resource_mutex);
	pthread_mutex_lock(&philo->shared_data->write);
	print_meal(philo);
	pthread_mutex_unlock(&philo->shared_data->write);
	ft_usleep(philo->shared_data, philo->individual_rules.t_eat);
	pthread_mutex_lock(&philo->shared_data->shared_resource_mutex);
	philo->remaining_meal--;
	pthread_mutex_unlock(&philo->shared_data->shared_resource_mutex);
}

/**
 * @function attempt_fork_lock_and_eat
 * @brief Tente de prendre des fourchettes et de manger.
 *
 * @param philo: Pointeur vers t_philo.
 * @param first_fork: Index de la première fourchette.
 * @param second_fork: Index de la seconde fourchette.
 *
 * @description Tente de verrouiller les deux fourchettes adjacentes. 
 * Si réussi, le philosophe consomme un repas.
 *
 * @return void
 *
 * @errors/edge_effects Utilise des mutex, peut bloquer.
 *
 * @examples attempt_fork_lock_and_eat(philo_ptr, fork1, fork2);
 *
 * @dependencies Dépend de consume_meal_and_update_time.
 *
 * @control_flow 1. Verrouiller les fourchettes.
 *               2. Si réussi, manger.
 *               3. Déverrouiller les fourchettes.
 */
void	attempt_fork_lock_and_eat(t_philo *philo, int first_fork,
	int second_fork)
{
	if (!pthread_mutex_lock(&philo->shared_data->forks[first_fork])
		&& !pthread_mutex_lock(&philo->shared_data->forks[second_fork]))
	{
		consume_meal_and_update_time(philo);
	}
	pthread_mutex_unlock(&philo->shared_data->forks[second_fork]);
	pthread_mutex_unlock(&philo->shared_data->forks[first_fork]);
}

/**
 * @function initiate_eating_process
 * @brief Lance le processus de prise de fourchettes et de manger pour un 
 * philosophe.
 *
 * @param philo: Pointeur vers la structure t_philo.
 *
 * @description Selon l'ID du philosophe, identifie les fourchettes adjacentes
 * et lance la tentative de les verrouiller pour manger.
 *
 * @return void
 *
 * @errors/edge_effects Aucun effet de bord.
 *
 * @examples initiate_eating_process(philo_ptr);
 *
 * @dependencies Dépend de la fonction attempt_fork_lock_and_eat.
 *
 * @control_flow 1. Vérifier si le philosophe est seul.
 *               2. Identifier les fourchettes adjacentes.
 *               3. Lancer la tentative de verrouillage et de manger.
 */
void	initiate_eating_process(t_philo *philo)
{
	int	first_fork;
	int	second_fork;

	if (philo->individual_rules.n_philo == ALONE)
		return ;
	if (philo->id % 2 == 0)
	{
		first_fork = philo->id - 1;
		second_fork = philo->id % philo->individual_rules.n_philo;
	}
	else if (philo->id % 2 != 0)
	{
		first_fork = philo->id % philo->individual_rules.n_philo;
		second_fork = philo->id - 1;
	}
	attempt_fork_lock_and_eat(philo, first_fork, second_fork);
}

/**
 * @function        print_action_and_wait
 * @brief           Affiche une action du philosophe et attend un temps donné.
 * @param t_philo   *philo - Pointeur vers la structure du philosophe.
 * @param const     *action - L'action à afficher.
 * @param int       time_to_action - Temps à attendre en millisecondes.
 * 
 * @description     Verrouille le mutex de l'écriture, vérifie si le philosophe
 *                  doit continuer et affiche l'action. Ensuite, déverrouille 
 *                  le mutex et fait attendre le thread pour time_to_action.
 * 
 * @return          Aucun.
 * 
 * @side_effect     Peut verrouiller et déverrouiller un mutex.
 * 
 * @example         print_action_and_wait(philo, "is eating", 500);
 * 
 * @dependencies    pthread_mutex_lock, pthread_mutex_unlock, printf, ft_usleep
 * 
 * @control_flow    1. Verrouiller le mutex.
 *                  2. Vérification de la condition de continuation.
 *                  3. Affichage de l'action.
 *                  4. Déverrouillage du mutex.
 *                  5. Appel de ft_usleep pour attendre.
 */
void	print_action_and_wait(t_philo *philo, const char *action,
	int time_to_action)
{
	pthread_mutex_lock(&philo->shared_data->write);
	if (!should_philo_continue(philo->shared_data, philo)
		&& philo->individual_rules.n_philo > 1)
		printf(action, get_time(philo->shared_data->t0),
			philo->id);
	pthread_mutex_unlock(&philo->shared_data->write);
	ft_usleep(philo->shared_data, time_to_action);
}

/**
 * @function        run_philosopher_routine
 * @brief           Définit la routine du philosophe dans le thread.
 * @param void      *valise - Pointeur générique vers les données du 
 * 					philosophe.
 * 
 * @description     Initialisée par pthread_create, cette fonction prend en 
 *                  paramètre une "valise" qui est un pointeur vers t_philo.
 *                  Elle exécute les actions de manger, dormir et penser.
 * 
 * @return          Pointeur vers void (NULL si le philosophe doit s'arrêter).
 * 
 * @side_effect     Modification de l'état du philosophe, affichage d'actions.
 * 
 * @example         pthread_create(&thread, NULL, run_philosopher_routine, 
 * 					valise);
 * 
 * @dependencies    usleep, initiate_eating_process, print_action_and_wait,
 *                  should_philo_continue
 * 
 * @control_flow    1. Conversion du pointeur valise en pointeur t_philo.
 *                  2. Pause conditionnelle si id est impair.
 *                  3. Boucle infinie pour réaliser les actions.
 *                  4. Vérification de la condition de continuation.
 */
void	*run_philosopher_routine(void *valise)
{
	t_philo	*philo;

	philo = (t_philo *) valise;
	if (philo->id % 2)
		usleep(philo->individual_rules.t_eat);
	while (INFINITY_LOOP)
	{
		initiate_eating_process(philo);
		print_action_and_wait(philo, LOG_SLEEPING,
			philo->individual_rules.t_sleep);
		print_action_and_wait(philo, LOG_THINKING,
			philo->individual_rules.t_think);
		if (should_philo_continue(philo->shared_data, philo))
			return ((void *) 0);
	}
}
