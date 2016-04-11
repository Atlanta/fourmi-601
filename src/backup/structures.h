#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

#include <pthread.h>


typedef struct case_tag{					/* Description d'une case sur la grille de simulation */
	pthread_t *fourmi;						/* Identifiant du thread de la fourmi presente sur la case */
	pthread_mutex_t mutex;					/* Protection de la case */
	int element;							/* Ce qui est present sur la case */


}case_t;

typedef struct coord_tag {					 /*  Coordonnees d'une case sur la grille de simulation*/
	int y;
	int x;
}coord_t;

#endif
