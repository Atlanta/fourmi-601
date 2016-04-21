#ifndef __OUTILEDITEUR_H__
#define __OUTILEDITEUR_H__

#include <ncurses.h>
#include "ncurses.h"
#include "../header/bibite.h"
#include "../header/fourmi.h"
#include "../header/fourmiliere.h"
#include "../header/gestionnaireNourriture.h"

#define ERREUR strerror(errno)

#define HAUTEUR 30
#define LARGEUR 100
#define HAUTEUR_INFO 10
#define LARGEUR_INFO 30
#define HAUTEUR_OUTIL 5
#define LARGEUR_OUTIL 30
#define HAUTEUR_CONFIG 11
#define LARGEUR_CONFIG 30

#define NB_TYPES_FOURMI_MAX 10 /* Totalement arbitraire, pour simplifier l'affichage */

#define VIDE 0
#define FOURMILIERE 1
#define OBSTACLE 2

#define AJOUTER 1
#define MODIFIER 2

#define ICONE_VIDE ' '
#define ICONE_FOURMILIERE ACS_DIAMOND
#define ICONE_OBSTACLE ACS_CKBOARD

WINDOW *cadreEditeur, *cadreInfos, *cadreOutil, *cadreConfig, *editeur, *infos, *config, *outil;
typedef struct fichier_zone {
	char grille[HAUTEUR*LARGEUR]; /* Buffer de la grille */
	unsigned char nbTypesFourmi;
	type_fourmi_t typesFourmi[NB_TYPES_FOURMI_MAX];
	unsigned char nbBibites;
	bibite_t* bibites;
	gestionnaire_nourriture_t gestionnaireNourriture;
	unsigned char nbFourmilieres;
	fourmiliere_t* fourmilieres;
} fichier_zone_t;
char outilActuel;

void handler(int sig);

void dessinerFenetreZone(fichier_zone_t* zone);
void dessinerFenetreInfos();
void dessinerFenetreOutils();
void dessinerFenetreConfig(fichier_zone_t* zone);

void initialiserFenetres();
void drawBottomBar();
void eraseBottomBar();
void refreshAll();

int getPosition(souris_t souris); /* Renvoie la position de la souris par rapport au clic dans la fenêtre de zone */
int isInZoneWindow(souris_t souris); /* Teste si l'utilisateur clique dans la fenêtre de zone */

void configurerTypeFourmi(fichier_zone_t* zone);

void changerOutil(char numeroOutil, char* outilActuel);
int chargerFichier(char* nomFichier, fichier_zone_t* zone);
int ecrireFichier(int fd, fichier_zone_t* zone);
int enregistrerFichier(char* nomFichier, fichier_zone_t* zone);

#endif
