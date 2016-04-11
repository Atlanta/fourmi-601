#ifndef __ZONE_H__
#define __ZONE_H__

typedef struct coord{
    int x;
    int y;
}coord_t;

typedef struct zone{
    int id_zone;
    coord_t* cases;
}zone_t;

/**
 * Fonction permettant la création d'une zone à partir d'une fichier d'initialisation
 * @param fd : descripteur du fichier d'initialisation
 * #return zone_t* : pointeur vers la zone créée
 */
zone_t* creerZone(int fd);





#endif
