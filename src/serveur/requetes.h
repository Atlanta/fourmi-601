#ifndef REQUETES__H
#define REQUETES__H

#include "serveur.h"

#define TYPE_CONNEXION 1
#define TYPE_FOURMI 2

typedef struct requete {
	short type;
} requete_t;

typedef struct reponse_connexion {
	short id;
	frontieres_t etatFrontieres;
} reponse_connexion_t;

#endif /* end of include guard: REQUETES__H */
