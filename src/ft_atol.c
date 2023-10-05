/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raveriss <raveriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 10:40:33 by raveriss          #+#    #+#             */
/*   Updated: 2023/10/05 18:56:07 by raveriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

/**
 * @function convert_to_long
 * @brief Convertit une chaîne de caractères en long.
 *
 * @param nptr: Chaîne à convertir.
 * @param i: Index de début pour la conversion.
 * @param sign: Signe de la valeur (-1 ou 1).
 * 
 * @description Convertit une chaîne en long en tenant compte des
 * limites. Gère les débordements.
 *
 * @return long: Retourne la valeur convertie.
 *
 * @errors/edge_effects Peut retourner LONG_MIN ou LONG_MAX en cas
 * de débordement.
 *
 * @examples long num = convert_to_long("123", 0, 1);
 *
 * @dependencies Aucune dépendance externe.
 *
 * @control_flow 1. Initialiser les variables.
 *               2. Boucler à travers la chaîne.
 *               3. Vérifier les limites et convertir.
 */
long	convert_to_long(const char *nptr, int i, long sign)
{
	long	nb;
	long	max_limit;

	nb = 0;
	max_limit = LONG_MAX;
	while (nptr[i] && nptr[i] >= '0' && nptr[i] <= '9')
	{
		if (nb > (max_limit - (nptr[i] - '0')) / 10)
		{
			if (sign == -1)
				return (LONG_MIN);
			else
				return (max_limit);
		}
		nb = nb * 10 + (nptr[i++] - '0');
	}
	return (sign * nb);
}

/**
 * @function ft_atol
 * @brief Convertit une chaîne de caractères en long.
 *
 * @param nptr: La chaîne à convertir.
 *
 * @description Passe les espaces blancs et vérifie le signe
 * avant de convertir la chaîne en un long.
 *
 * @return long: Retourne le long converti.
 *
 * @errors/edge_effects Peut retourner LONG_MIN ou LONG_MAX si
 * débordement.
 *
 * @examples long num = ft_atol("-12345");
 *
 * @dependencies Dépend de convert_to_long.
 *
 * @control_flow 1. Ignorer les espaces blancs.
 *               2. Déterminer le signe.
 *               3. Appeler convert_to_long.
 */
long	ft_atol(const char *nptr)
{
	int		i;
	long	sign;

	i = 0;
	sign = 1;
	while ((nptr[i] >= TAB_CHAR && nptr[i] <= CARRIAGE_RETURN)
		|| nptr[i] == SPACE_CHAR)
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		else
			sign = 1;
		i++;
	}
	return (convert_to_long(nptr, i, sign));
}
