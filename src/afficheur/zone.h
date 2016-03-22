#ifndef ZONE_H
#define ZONE_H

#include <pthread.h>
#include "structures.h"

#define NB_LIGNES_SIM		30				/* Dimensions des fenetres du programme */
#define NB_COL_SIM			100
#define NB_LIGNES_MSG		29
#define NB_COL_MSG			49
#define NB_LIGNES_OUTILS	9
#define NB_COL_OUTILS		49

#define MAX_FOURMIS			10				/* Nombre maximum de fourmis de la simulation */

#define VIDE				0
#define FOURMI				1
#define OBSTACLE			2
#define NOURRITURE 			3
#define BIBITE 				4
#define FOURMILIERE			5

#define HAUT	0
#define BAS		1
#define GAUCHE	2
#define DROITE 	3

typedef struct case_tag {					/* Description d'une case sur la grille de simulation */
	int element;							/* Ce qui est present sur la case */
	pthread_t *fourmi;						/* Identifiant du thread de la fourmi presente sur la case */
	pthread_mutex_t mutex;					/* Protection de la case */
} case_t;

pthread_t *threads_fourmis[MAX_FOURMIS];	/* Identifants des threads des fourmis de la simulation*/

case_t grille[NB_LIGNES_SIM][NB_COL_SIM];	/* Grille de simulation */

#endif
