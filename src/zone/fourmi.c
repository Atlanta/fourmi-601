#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include "fourmi.h"

void* routineFourmi(void *args) {
	coord_t* coord = (coord_t*) args;
	case_t* caseSuivante;

	srand(time(NULL));

	while (1) {
		/*Que feront les fourmis ?!?!?!*/
		switch (rand() % 4) {
			case HAUT:
				if(coord->y > 0) {
					caseSuivante = &grille[coord->y-1][coord->x];
					if(pthread_mutex_trylock(&caseSuivante->mutex) == 0)
						if(caseSuivante->element == VIDE) {
							caseSuivante->fourmi = grille[coord->y][coord->x].fourmi;
							grille[coord->y][coord->x].element = VIDE;
							caseSuivante->element = FOURMI;
							grille[coord->y][coord->x].fourmi = NULL;
							coord->y--;
							pthread_mutex_unlock(&caseSuivante->mutex);
						}
				}
				break;
			case BAS:
				if(coord->y < NB_LIGNES_SIM-1) {
					caseSuivante = &grille[coord->y+1][coord->x];
					if(pthread_mutex_trylock(&caseSuivante->mutex) == 0)
						if(caseSuivante->element == VIDE) {
							caseSuivante->fourmi = grille[coord->y][coord->x].fourmi;
							grille[coord->y][coord->x].element = VIDE;
							caseSuivante->element = FOURMI;
							grille[coord->y][coord->x].fourmi = NULL;
							coord->y++;
							pthread_mutex_unlock(&caseSuivante->mutex);
						}
				}
				break;
			case GAUCHE:
				if(coord->x > 0) {
					caseSuivante = &grille[coord->y][coord->x-1];
					if(pthread_mutex_trylock(&caseSuivante->mutex) == 0)
						if(caseSuivante->element == VIDE) {
							caseSuivante->fourmi = grille[coord->y][coord->x].fourmi;
							grille[coord->y][coord->x].element = VIDE;
							caseSuivante->element = FOURMI;
							grille[coord->y][coord->x].fourmi = NULL;
							coord->x--;
							pthread_mutex_unlock(&caseSuivante->mutex);
						}
				}
				break;
			case DROITE:
				if(coord->x < NB_COL_SIM-1) {
					caseSuivante = &grille[coord->y][coord->x+1];
					if(pthread_mutex_trylock(&caseSuivante->mutex) == 0)
						if(caseSuivante->element == VIDE) {
							caseSuivante->fourmi = grille[coord->y][coord->x].fourmi;
							grille[coord->y][coord->x].element = VIDE;
							caseSuivante->element = FOURMI;
							grille[coord->y][coord->x].fourmi = NULL;
							coord->x++;
							pthread_mutex_unlock(&caseSuivante->mutex);
						}
				}
				break;
		}
		sleep(1);
	}

	free(coord);
	return NULL;
}
