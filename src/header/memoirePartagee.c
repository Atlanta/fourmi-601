#include <stdio.h>      /* Pour printf, perror */
#include <stdlib.h>     /* Pour exit, EXIT_SUCCESS, EXIT_FAILURE */
#include <sys/shm.h>    /* Pour shmget, shmat, shmdt */
#include <errno.h>      /* Pour errno */
#include <sys/stat.h>   /* Pour S_IRUSR, S_IWUSR */
#include "memoirePartagee.h"


segment_t* creerTerrain(key_t cle, int hauteur, int largeur) {
	int i;
	int shmid;
	void* addr;
	segment_t *terrain;


	terrain = (segment_t*)malloc(sizeof(segment_t));

	if((shmid = shmget(cle, 2 * sizeof(int) + hauteur * largeur * sizeof(int), S_IRUSR | S_IWUSR | IPC_CREAT)) == -1) {
		perror("Erreur de création du segment ");
		exit(EXIT_FAILURE);
	}

	addr = shmat(shmid, NULL, 0);
	terrain->hauteur = (int*) addr;
	terrain->largeur = (int*) &terrain->hauteur[1];
	terrain->cases = (int*) &terrain->largeur[1];
	*(terrain->largeur) = largeur;
	*(terrain->hauteur) = hauteur;

	for(i = 0; i < (int)(*terrain->hauteur)*(int)(*terrain->largeur); i++) {
            /* TODO : assigner les valeurs au tableau */
			terrain->cases[i] = 0;
	}

	return terrain;
}

segment_t* recupererTerrain(key_t cle) {
	int shmid;
	void* addr;
	segment_t *terrain;

	terrain = (segment_t*)malloc(sizeof(segment_t));

	if((shmid = shmget(cle, 0, 0)) == -1) {
		perror("Erreur de récupération du segment ");
		exit(EXIT_FAILURE);
	}

	addr = shmat(shmid, NULL, 0);
	terrain->hauteur = (int*) addr;
	terrain->largeur = (int*) &terrain->hauteur[1];
	terrain->cases = (int*) &terrain->largeur[1];
	return terrain;
}

void supprimerTerrain(key_t cle) {
	int shmid;

	if((shmid = shmget(cle, 0, 0)) == -1) {
		perror("Erreur lors de la recuperation du segment de memoire partagee ");
		exit(EXIT_FAILURE);
	}

	if(shmctl(shmid, IPC_RMID, 0) == -1) {
		perror("Erreur lors de la suppression du segment de memoire partagee ");
		exit(EXIT_FAILURE);
	}
}
