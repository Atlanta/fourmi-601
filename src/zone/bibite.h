/* Bibites
 Structures et fonctions*/

#include "fourmi.h"

#define PACIFIQUE 1
#define BELLIQUEUSE 2

typedef struct bibite{
    int pv_max;
    int pv_actuel;
    int pts_attaque;
    int type; /* PACIFIQUE || BELLIQUEUSE*/
} bibite_t;

bibite_t* creerBibite(int fd, int type);

bibite_t* copierBibite(bibite_t* other);

/* Fonction de déplacement de la bibite
 Fonction appelée à la création d'un thread
 args = coordonnées de départ de la bibite (coord_t*) */
void* deplacer(void* args);

/* Attaque une fourmi
 dégats de l'attaque = jet de dés sur les pts_attaque de la bibite
 degats = rand()% pts_attaque; */
void bibiteAttaquer(bibite_t* bibite, fourmi_t* enemmi);

/* Transforme une bibitte en source de nourriture */
void bibiteMourir(bibite_t* bibite);

void routineBibite();