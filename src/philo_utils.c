/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raveriss <raveriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:47:17 by raveriss          #+#    #+#             */
/*   Updated: 2023/10/05 19:10:19 by raveriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

/**
 * @function        ft_atoi
 * @brief           Convertit une chaîne en un entier signé.
 * @param const     *nptr - Pointeur vers la chaîne à convertir.
 * 
 * @description     Saute les espaces et les caractères de contrôle initiaux,
 *                  détecte le signe éventuel, et effectue la conversion en 
 * 					entier.
 * 
 * @return          Entier converti à partir de la chaîne d'entrée.
 * 
 * @side_effect     Aucun.
 * 
 * @example         int num = ft_atoi("  -42");
 * 
 * @dependencies    Aucune.
 * 
 * @control_flow    1. Initialisation des variables.
 *                  2. Skip des espaces et caractères de contrôle.
 *                  3. Détection du signe.
 *                  4. Conversion en entier.
 */
int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	nb;

	i = 0;
	sign = 1;
	nb = 0;
	while ((nptr[i] >= TAB_CHAR && nptr[i] <= CARRIAGE_RETURN)
		|| nptr[i] == SPACE_CHAR)
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -sign;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		nb = nb * 10 + (nptr[i] - '0');
		i++;
	}
	return (sign * nb);
}

/**
 * @function        get_time
 * @brief           Obtient le temps actuel depuis une origine t0.
 * @param long      t0 - Le temps d'origine en millisecondes.
 * 
 * @description     Utilise gettimeofday pour récupérer le temps actuel, puis le
 *                  convertit en millisecondes et soustrait t0.
 * 
 * @return          Le temps écoulé depuis t0 en millisecondes.
 * 
 * @side_effect     Aucun.
 * 
 * @example         long elapsed = get_time(1633045622L);
 * 
 * @dependencies    gettimeofday
 * 
 * @control_flow    1. Obtenir le temps actuel via gettimeofday.
 *                  2. Convertir en millisecondes.
 *                  3. Soustraire t0.
 */
long	get_time(long t0)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_usec / US_TO_MS + tv.tv_sec * SEC_TO_MS - t0);
}

/**
 * @function        ft_usleep
 * @brief           Met en pause l'exécution pendant une durée.
 * @param t_data    *data - Pointeur vers les données partagées.
 * @param long      duration - Durée de la pause en millisecondes.
 * 
 * @description     Met le thread en pause pendant 'duration' millisecondes,
 *                  tout en vérifiant si le philosophe doit continuer.
 * 
 * @return          Aucun.
 * 
 * @side_effect     Pause dans l'exécution du thread.
 * 
 * @example         ft_usleep(shared_data, 500);
 * 
 * @dependencies    get_time, usleep, should_philo_continue
 * 
 * @control_flow    1. Enregistrement du temps de départ.
 *                  2. Boucle jusqu'à ce que la durée soit écoulée.
 *                  3. Appels de usleep et mise à jour du temps actuel.
 */
void	ft_usleep(t_data *data, long duration)
{
	long	start_t;
	long	cur_t;

	start_t = get_time(0);
	cur_t = start_t;
	while (!should_philo_continue(data, &data->philo[0])
		&& (cur_t - start_t) < duration)
	{
		usleep(100);
		cur_t = get_time(0);
	}
}

/**
 * @function        should_philo_continue
 * @brief           Évalue si un philosophe doit continuer à agir.
 * @param t_data    *data - Pointeur vers les données partagées.
 * @param t_philo   *philo - Pointeur vers le philosophe à évaluer.
 * 
 * @description     Verifie les conditions de fin : si quelqu'un est mort ou si 
 *                  le philosophe a terminé ses repas.
 * 
 * @return          Retourne SUCCESS ou FAILURE.
 * 
 * @side_effect     Utilisation d'un mutex pour accéder à la ressource partagée.
 * 
 * @example         if (should_philo_continue(data, &philo))
 * 
 * @dependencies    pthread_mutex_lock, pthread_mutex_unlock
 * 
 * @control_flow    1. Verrouillage du mutex.
 *                  2. Vérification des conditions.
 *                  3. Déverrouillage du mutex.
 */
int	should_philo_continue(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(&data->shared_resource_mutex);
	if (data->someone_died || !philo->remaining_meal)
	{
		pthread_mutex_unlock(&data->shared_resource_mutex);
		return (FAILURE);
	}
	pthread_mutex_unlock(&data->shared_resource_mutex);
	return (SUCCESS);
}

/**
 * @function        print_meal
 * @brief           Imprime les actions de repas du philosophe.
 * @param t_philo   *philo - Pointeur vers le philosophe concerné.
 * 
 * @description     Imprime les actions du philosophe liées au repas si le 
 *                  philosophe doit continuer à agir.
 * 
 * @return          Aucun.
 * 
 * @side_effect     Impression dans la console.
 * 
 * @example         print_meal(&philo);
 * 
 * @dependencies    printf, should_philo_continue
 * 
 * @control_flow    1. Vérification de la condition de continuation.
 *                  2. Impressions successives liées au repas.
 */
void	print_meal(t_philo *philo)
{
	if (!should_philo_continue(philo->shared_data, philo))
	{
		printf(LOG_FORK, philo->last_meal, philo->id);
		printf(LOG_FORK, philo->last_meal, philo->id);
		printf(LOG_EAT, philo->last_meal, philo->id);
	}
}
