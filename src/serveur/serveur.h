#ifndef __SERVEUR_H__
#define __SERVEUR_H__

#include <stdlib.h>

#ifndef _LOG
#define _LOG(x) printf("[%d] ", getpid());printf(x);printf("\n");
#endif

#define HAUT 0
#define BAS 1
#define GAUCHE 2
#define DROITE 3

#define HAUTEUR 5
#define LARGEUR 5

#define NB_MAX_CONNEXION 25

#define PORT_CONNEXION 5555
#define PORT_MESSAGES 5556

#define VIDE 0
#define OCCUPE 1
#define INEXISTANT 2

/* Inexistant : 2; Occupé : 1; libre : 0; */
typedef struct frontieres {
	char haut;
	char bas;
	char gauche;
	char droite;
} frontieres_t;

int grille[HAUTEUR*LARGEUR];

int choisirCaseVide();
frontieres_t calculerFrontieres(int place);

#endif /* end of include guard: __SERVEUR_H__ */
