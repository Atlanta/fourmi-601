#ifndef __NOURITURE_H__
#define __NOURITURE_H__

#include "zone.h"

typedef struct nourriture{
    coord_t* position;
    int montantNourriture;
}nourriture_t;

/** Fonction de création d'une source de nourriture a partir d'un fichier d'initialisation
 * @param fd : descripteur du fichier d'initialisation
 * @return nourriture_t* : pointeur sur la structure de nourriture créée
 */
nourriture_t* creerNourritureViaFichier(int fd);

/** Fonction de création d'une source de nourriture
 * @param position : type coord_t*, position de la source de nourriture
 * @param montantNourriture : montant de nourriture disponible
 * @return nourriture_t* : poitneur sur la structure de nourriture créée
 */
nourriture_t* creerNourriture(coord_t* position, int montantNourriture);


/** Fonction de destruction d'une source de nourriture
 * @param nourriture : source à détruire
*/
void detruireNourriture(nourriture_t* nourriture);


#endif
