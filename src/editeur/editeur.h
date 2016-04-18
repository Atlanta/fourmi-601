#ifndef __EDITEUR_H__
#define __EDITEUR_H__

#include <ncurses.h>
#include "ncurses.h"

#define HAUTEUR 30
#define LARGEUR 100
#define HAUTEURINFO 19
#define LARGEURINFO 30
#define HAUTEUROUTIL 9
#define LARGEUROUTIL 30

#define VIDE 0
#define FOURMI 1
#define OBSTACLE 2

#define ICONE_VIDE " "
#define ICONE_FOURMI "@"
#define ICONE_OBSTACLE "#"

WINDOW *cadreEditeur, *cadreInfos, *cadreOutil, *editeur, *infos, *outil;
char outilActuel;

void initialiserFenetres();
void initialiserFichier(int fd);
void chargerFichier(int fd);
void changerOutil(char numeroOutil);
void refreshAll();
int getFilePosition(souris_t souris); /* Renvoie la position dans le fichier par rapport au clic dans la fenêtre de zone */
int isInZoneWindow(souris_t souris); /* Teste si l'utilisateur clique dans la fenêtre de zone */
/*void enregistrer(int fd);*/
void terminer();

#endif
