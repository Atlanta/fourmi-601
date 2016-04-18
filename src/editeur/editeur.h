#ifndef __EDITEUR_H__
#define __EDITEUR_H__

#include <ncurses.h>
#include "ncurses.h"

#define HAUTEUR 30
#define LARGEUR 100
#define HAUTEURINFO 30
#define LARGEURINFO 30

#define VIDE 0
#define FOURMI 1
#define OBSTACLE 2

#define ICONE_FOURMI ACS_DIAMOND
#define ICONE_OBSTACLE ACS_CKBOARD

WINDOW *cadreEditeur, *cadreInfos, *editeur, *infos;

void initialiserFichier(int fd);
void chargerFichier(int fd);
void changerOutil(int numeroOutil);
void refreshAll();
int getFilePosition(souris_t souris); /* Renvoie la position dans le fichier par rapport au clic dans la fenêtre de zone */
int isInZoneWindow(souris_t souris); /* Teste si l'utilisateur clique dans la fenêtre de zone */
/*void enregistrer(int fd);*/

#endif
