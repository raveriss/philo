/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raveriss <raveriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 10:20:26 by raveriss          #+#    #+#             */
/*   Updated: 2023/10/05 19:13:32 by raveriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

/**
 * @function        initialize_and_start
 * @brief           Initialise et lance les threads de philosophes.
 * @param t_data    *data - Pointeur vers les données partagées.
 * 
 * @description     Initialise le temps de départ et crée les threads pour
 *                  chaque philosophe. Rejoint ensuite ces threads.
 * 
 * @return          Retourne SUCCESS ou les codes d'échec.
 * 
 * @side_effect     Création et terminaison de threads.
 * 
 * @example         if (initialize_and_start(&data) != SUCCESS)
 * 
 * @dependencies    get_time, pthread_create, pthread_join, 
 *                  check_philosopher_life
 * 
 * @control_flow    1. Initialisation du temps de départ.
 *                  2. Création des threads.
 *                  3. Vérification de la vie des philosophes.
 *                  4. Jointure des threads.
 */
int	initialize_and_start(t_data *data)
{
	int	i;

	i = 0;
	data->t0 = get_time(0);
	while (i < data->global_rules.n_philo)
	{
		if (pthread_create(&data->thread_ids[i], NULL,
				&run_philosopher_routine, &data->philo[i]))
			return (FAILURE_CREAT_THREADS);
		i++;
	}
	check_philosopher_life(data);
	i = 0;
	while (i < data->global_rules.n_philo)
	{
		if (pthread_join(data->thread_ids[i], NULL))
			return (FAILURE_JOIN_THREADS);
		i++;
	}
	return (SUCCESS);
}

/**
 * @function        check_arg
 * @brief           Vérifie la validité des arguments passés.
 * @param int       argc - Nombre d'arguments.
 * @param char      **argv - Tableau des arguments.
 * 
 * @description     Vérifie le nombre d'arguments et leurs valeurs pour s'assurer
 *                  qu'ils sont valides selon les contraintes du programme.
 * 
 * @return          Retourne SUCCESS ou FAILURE.
 * 
 * @side_effect     Aucun.
 * 
 * @example         if (check_arg(argc, argv) != SUCCESS)
 * 
 * @dependencies    ft_atoi, ft_atol
 * 
 * @control_flow    1. Vérification du nombre d'arguments.
 *                  2. Vérification des limites des arguments.
 *                  3. Vérification des types des arguments.
 */
int	check_arg(int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
		return (FAILURE);
	if (ft_atol(argv[1]) > IMAX || ft_atol(argv[2]) > IMAX
		|| ft_atol(argv[3]) > IMAX || ft_atol(argv[4]) > IMAX
		|| (argv[5] && ft_atol(argv[5]) > IMAX))
		return (FAILURE);
	if (ft_atoi(argv[1]) <= 0 || ft_atoi(argv[2]) <= 0
		|| ft_atoi(argv[3]) <= 0 || ft_atoi(argv[4]) <= 0
		|| (argv[1] && ft_atoi(argv[1]) <= 0))
		return (FAILURE);
	while (argc > 1)
	{
		i = 0;
		while (argv[argc - 1][i])
		{
			if (!(argv[argc - 1][i] >= '0' && argv[argc - 1][i] <= '9'))
				return (FAILURE);
			i++;
		}
		argc--;
	}
	return (SUCCESS);
}

/**
 * @function        main
 * @brief           Point d'entrée principal du programme.
 * @param int       argc - Nombre d'arguments.
 * @param char      **argv - Tableau des arguments.
 * 
 * @description     Initialise les données, les vérifie et gère le flux du 
 *                  programme de la création à la fin des threads.
 * 
 * @return          Retourne SUCCESS ou les codes d'erreur.
 * 
 * @side_effect     Impression des messages d'erreur, modification des données.
 * 
 * @example         ./executable <args>
 * 
 * @dependencies    check_arg, init, initialize_and_start, cleanup_and_end
 * 
 * @control_flow    1. Vérification des arguments.
 *                  2. Initialisation des données.
 *                  3. Création et gestion des threads.
 *                  4. Nettoyage et terminaison.
 */
int	main(int argc, char **argv)
{
	t_data	data;
	int		error;

	data.someone_died = 0;
	if (check_arg(argc, argv))
		return (printf(ERR_ARGS));
	error = 0;
	error = init(&data, argv);
	if (error)
		return (printf(ERR_ALLOC));
	error = initialize_and_start(&data);
	if (error == FAILURE_CREAT_THREADS)
		return (printf(ERR_CREAT_THREADS));
	if (error == FAILURE_JOIN_THREADS)
		return (printf(ERR_JOIN_THREADS));
	error = cleanup_and_end(&data);
	if (error)
		return (printf(ERR_MUTEX_OPEN));
	return (SUCCESS);
}
