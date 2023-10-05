# Projet Philosophers

## Description
Ce projet est une implémentation du problème des philosophes à l'aide de la programmation multithreadée et la gestion de mutex en C. Le but est de résoudre les problèmes de concurrence comme les conditions de course, les deadlocks et les famines.

## Tableau des Composants du Projet

| Fichier             | Description                                                             | Responsabilité                                  | Statut    |
|---------------------|-------------------------------------------------------------------------|-------------------------------------------------|-----------|
| `philo.c`           | Fichier principal, contient la logique de haut niveau                   | Initialisation, gestion des threads             | Complet   |
| `init.c`            | Initialise les structures et les variables                              | Initialisation                                  | Complet   |
| `life.c`            | Gère la vie des philosophes                                             | Manger, dormir, penser                           | Complet   |
| `check_death.c`     | Vérifie si un philosophe est mort                                       | Surveillance de la mort                          | Complet   |
| `philo_utils.c`     | Fonctions utilitaires                                                    | Fonctions auxiliaires                            | Complet   |
| `ft_atol.c`         | Conversion de chaîne à long int                                         | Conversion de types                              | Complet   |
| `clean.c`           | Nettoie les ressources avant de quitter le programme                    | Nettoyage                                        | Complet   |
| `Makefile`          | Fichier pour compiler le projet                                          | Compilation                                      | Complet   |
| `philo.h`           | Fichier d'en-tête contenant les déclarations et les macros               | Déclarations                                    | Complet   |



## Installation et Compilation
```
bash
Copy code
git clone [Votre URL de dépôt]
cd [Nom du répertoire]
make
```

## Utilisation
```
bash
Copy code
./philo [options]
```

## Options
option1: Description de l'option 1
option2: Description de l'option 2

## Caractéristiques
Gestion de la concurrence avec pthread et mutex
Robustesse et absence de fuites de mémoire
Respect des normes de l'école 42

## Fonctions externes autorisées
memset
printf
malloc
free
write
usleep
gettimeofday
pthread_create
pthread_detach
pthread_join
pthread_mutex_init
pthread_mutex_destroy
pthread_mutex_lock
pthread_mutex_unlock

Auteur
raveriss
