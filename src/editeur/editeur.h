#ifndef __EDITEUR_H__
#define __EDITEUR_H__

#include <ncurses.h>
#include "ncurses.h"
#include "../zone/fourmi.h"

#define HAUTEUR 30
#define LARGEUR 100
#define HAUTEURINFO 19
#define LARGEURINFO 30
#define HAUTEUROUTIL 9
#define LARGEUROUTIL 30

#define VIDE 0
#define FOURMILIERE 1
#define OBSTACLE 2

#define ICONE_VIDE ' '
#define ICONE_FOURMILIERE ACS_DIAMOND
#define ICONE_OBSTACLE ACS_CKBOARD

WINDOW *cadreEditeur, *cadreInfos, *cadreOutil, *editeur, *infos, *outil;
char grille[HAUTEUR*LARGEUR]; /* Buffer de la grille */
char outilActuel;
unsigned char nbTypesFourmi;
type_fourmi_t* typesFourmi;
unsigned char nbBibites;
unsigned short periodiciteNourriture;

void handler(int sig);
void drawBottomBar();
void eraseBottomBar();
void initialiserFenetres();
void initialiserFichier(int fd);
void chargerFichier(char* nomFichier);
void changerOutil(char numeroOutil);
void refreshAll();
int getPosition(souris_t souris); /* Renvoie la position de la souris par rapport au clic dans la fenêtre de zone */
int isInZoneWindow(souris_t souris); /* Teste si l'utilisateur clique dans la fenêtre de zone */
void ecrireFichier(int fd);
void enregistrerFichier(char* nomFichier);
void terminer();

#endif
