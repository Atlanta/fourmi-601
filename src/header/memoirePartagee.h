#ifndef __MEMOIRE_PARTAGEE_H__
#define __MEMOIRE_PARTAGEE_H__

#define _XOPEN_SOURCE
#include <sys/shm.h>    /* Pour shmget, shmat, shmdt */

typedef struct segment{
    int* hauteur;
    int* largeur;
    int* cases;
}segment_t;

/** Fonction permettant la creation d'un segment de memoire partagée
 *  Ce segment contients les informations du terrain
 *  @param cle : identifiant du segment de memoire
 *  @param hauteur : hauteur (nombre de lignes) du terrain
 *  @param largeur : largeur (nombre de colonnes) du terrain
 *  @return segment_t* : pointeur vers la structure crée
 */
segment_t* creerTerrain(key_t cle, int hauteur, int largeur);

/** Fonction permettant à un client de récupérer le segment de memoire partagée
 *  @param cle : identifiant du segment de memoire
 *  @return segment_t* : pointeur vers la structure recuperée
 */
segment_t* recupererTerrain(key_t cle);

/** Fonction permettant de supprimer un segment de memoire partagée
  * @param cle : identifiant du segment de mémemoire
  */
void detruireTerrain(key_t cle);


#endif
