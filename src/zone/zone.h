#ifndef __ZONE_H__
#define __ZONE_H__

#define HAUTEUR 30
#define LARGEUR 100

#include "../header/semaphores.h"
#include "../header/memoirePartagee.h"

typedef struct coord{
    int x;
    int y;
}coord_t;

/*typedef struct zone{
    coord_t* cases;
    int id_zone;
}zone_t;*/

/**
 * Fonction permettant la création d'une zone à partir d'une fichier d'initialisation
 * @param fd : descripteur du fichier d'initialisation
 * #return zone_t* : pointeur vers la zone créée
 */
/* A revoir */
/*zone_t* creerZone(int fd);*/

void gestionnaire (int signum);





#endif
