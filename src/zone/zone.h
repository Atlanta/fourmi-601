#ifndef __ZONE_H__
#define __ZONE_H__

#define _XOPEN_SOURCE

#include <pthread.h>
#include <sys/ipc.h>

#define NBFOURMIS			10

#define LARGEUR 			100
#define HAUTEUR 			30

#define VIDE				0
#define FOURMILIERE			1
#define OBSTACLE			2
#define NOURRITURE 			3
#define BIBITE 				4
#define FOURMI				5

#define HAUT	0
#define BAS		1
#define GAUCHE	2
#define DROITE 	3

typedef struct case_tag {					/* Description d'une case sur la grille de simulation */
	unsigned char element;					/* Ce qui est present sur la case */
	pthread_t* elementThread;				/* Identifiant du thread de l'élement present sur la case (si besoin) */
	pthread_mutex_t mutex;					/* Protection de la case */
} case_t;

pthread_t *threads_fourmis[NBFOURMIS];	/* Identifants des threads des fourmis de la simulation*/
case_t grille[HAUTEUR][LARGEUR];

typedef struct zone {
	size_t* hauteur;
	size_t* largeur;
	case_t* grille;
} zone_t;

unsigned char* analyserZone(int fd); 	/* Crée le tableau des éléments d'un terrain à partir d'un fichier */
zone_t* creerZone(int fd, key_t cle); 	/* Renvoie un pointeur vers la zone de mémoire partagée créée */
zone_t* recupererZone(key_t cle);		/* Rencoie un pointeur vers une zone existante */
void initialiserZone(zone_t* zone);		/* Initialise les threads et les valeurs de la zone pointée */
/*void simulation_initialiser();*/

#endif
