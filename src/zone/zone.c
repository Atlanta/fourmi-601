#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>		/* Pour rand */
#include <unistd.h>
#include <errno.h>		/* Pour errno */
#include <sys/ipc.h>	/* Pour les IPC */
#include <sys/shm.h>	/* Pour les segments de mémoire partagés */
#include <sys/stat.h>	/* Pour les constantes */
#include <fcntl.h>
#include "fourmi.h"
#include "zone.h"

/*void simulation_initialiser() {
	int i, j;

	for (i = 0; i < NBFOURMIS; i++)
		threads_fourmis[i] = NULL;

	for (i = 0; i < HAUTEUR; i++) {
		for (j = 0; j < LARGEUR; j++) {
			grille[i][j].element = VIDE;
			grille[i][j].fourmi = NULL;
			pthread_mutex_init(&grille[i][j].mutex, NULL);
		}
	}
}

void simulation_stopper() {
	int i;

	for (i = 0; i < NBFOURMIS; i++) {
		if (threads_fourmis[i] != NULL) {
			pthread_cancel(*threads_fourmis[i]);
			threads_fourmis[i] = NULL;
		}
	}
}*/

unsigned char* analyserZone(int fd) {
	/* TODO Fonction qui analyse un fichier de zone */
	size_t i;
	unsigned char caseBuffer;
	unsigned char* zone = (unsigned char*) malloc(HAUTEUR*LARGEUR*sizeof(unsigned char));

	lseek(fd, 0, SEEK_SET);

	for(i = 0; i < HAUTEUR*LARGEUR; i++) {
		if(read(fd, &caseBuffer, sizeof(unsigned char)) == -1) {
			perror("Erreur de lecture du fichier ");
			exit(EXIT_FAILURE);
		}
		zone[i] = caseBuffer;
	}

	return zone;
}

zone_t* creerZone(int fd, key_t cle) {
	size_t i;
	int shmid;
	void* addr;
	zone_t* zone;
	unsigned char* grilleFichier;

	srand(time(NULL));

	zone = (zone_t*) malloc(sizeof(zone_t));

	if((shmid = shmget(cle, HAUTEUR * LARGEUR * sizeof(case_t), S_IRUSR | S_IWUSR | IPC_CREAT)) == -1) {
		perror("Erreur de création du segment ");
		exit(EXIT_FAILURE);
	}

	addr = shmat(shmid, NULL, 0);
	zone->hauteur = (size_t*) addr;
	zone->largeur = (size_t*) zone->hauteur[1];
	zone->grille = (case_t*) zone->largeur[1];

	*zone->hauteur = (size_t)HAUTEUR;
	*zone->largeur = (size_t)LARGEUR;

	grilleFichier = analyserZone(fd);

	for(i = 0; i < HAUTEUR*LARGEUR; i++) {
		zone->grille[i].element = grilleFichier[i];
	}

	return zone;
}

zone_t* recupererZone(key_t cle) {
	int shmid;
	void* addr;
	zone_t* zone;

	zone = (zone_t*)malloc(sizeof(zone_t));

	if((shmid = shmget(cle, 0, 0)) == -1) {
		perror("Erreur de récupération du segment ");
		exit(EXIT_FAILURE);
	}

	addr = shmat(shmid, NULL, 0);
	zone->hauteur = (size_t*) addr;
	zone->largeur = (size_t*) zone->hauteur[1];
	zone->grille = (case_t*) zone->largeur[1];

	return zone;
}

void initialiserZone(zone_t* zone) {
	
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Paramètres de la commande incorrectes. La commande est de la forme : %s <fichier_zone>\n", argv[0]);
		fprintf(stderr, "Où :\n");
		fprintf(stderr, "\t<fichier_zone> : Le chemin du fichier de zone à charger.\n");
		exit(EXIT_FAILURE);
	}

	/*simulation_initialiser();*/

	exit(EXIT_SUCCESS);
}
