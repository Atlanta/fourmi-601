#ifndef FOURMI_H
#define FOURMI_H

#include <pthread.h>

#define NB_LIGNES_SIM		40				/* Dimensions des fenetres du programme */
#define NB_COL_SIM			80
#define NB_LIGNES_MSG		29
#define NB_COL_MSG			49
#define NB_LIGNES_OUTILS	9
#define NB_COL_OUTILS		49

#define MAX_FOURMIS			10				/* Nombre maximum de fourmis de la simulation */

#define VIDE				0				/* Identifiants des elements pouvant etre */
#define OBSTACLE			1				/* places sur la grille de simulation */
#define FOURMI				2

#define HAUT	0
#define BAS		1
#define GAUCHE	2
#define DROITE 	3

typedef struct case_tag {					/* Description d'une case sur la grille de simulation */
	int element;							/* Ce qui est present sur la case */
	pthread_t *fourmi;						/* Identifiant du thread de la fourmi presente sur la case */
	pthread_mutex_t mutex;					/* Protection de la case */
} case_t;

typedef struct coord_tag {					/* Coordonnees d'une case sur la grille de simulation*/
	int y;
	int x;
} coord_t;


#endif
