/* Bibites
 Structures et fonctions*/

#define PACIFIQUE 1
#define BELLIQUEUSE 2

typedef struct bibite{
    int pv_max;
    int pv_actuel;
    int pts_attaque;
    int type; /* PACIFIQUE || BELLIQUEUSE*/
} bibite_t;

/* Création d'une bibite de référence à partir du fichier d'initialisation
 fd = descripteur de fichier */
bibite_t* creerBibitePacifiqueReference(int fd);

/*Crée une bibite et la place sur la zone
 La bibite contient les mêmes caractéristiques que la bibite de réference */
bibite_t* creerBibitePacifique(bibite_t* referencePacifique);

/* Création d'une bibite de référence à partir du fichier d'initialisation
 fd = descripteur de fichier */
bibite_t* creerBibiteBelliqueuseReference(int fd);

/*Crée une bibite et la place sur la zone
 La bibite contient les mêmes caractéristiques que la bibite de réference */
bibite_t* creerBibiteBelliqueuse(bibite_t* referenceBelliqueuse);


/* Fonction de déplacement de la bibite
 Fonction appelée à la création d'un thread
 args = coordonnées de départ de la bibite (coord_t*) */
void* deplacer(void* args);


/* Attaque une fourmi
 dégats de l'attaque = jet de dés sur les pts_attaque de la bibite
 degats = rand()% pts_attaque; */
void attaquer(fourmi_t* enemmi);

/* Transforme une bibitte en source de nourriture */
void mourrir(bibite_t* bibite);
