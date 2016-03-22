/* Fourmis
Structures et fonctions: */

#include "structures.h"

#define SOLDATE 1
#define OUVRIERE 2

typedef struct fourmi {
    /* coord_t = struct contenant les positons X Y */
    coord_t* position_fourmiliere;
    /* Permet à la fourmi de savoir d'où elle vient */
    int id_zone;

	int id_fourmiliere;
	/* Points de vie maximums */
    int pv_max;
	/* Points de vie restants */
    int pv_actuel;
	/* Quantite de nourriture que la fourmi peut stocker (si elle n'a pas besoin de points de vie) */
    int capacite_stockage;
	/* Valeur de dégats */
    int pts_attaque;
    int type; /* SOLDATE || OUVRIERE */
} fourmi_t;

/**
 * Crée une fourmis selon le fichier de configuration
 * @param fd descripteur de fichier
 * @return fourmi créée
 */
fourmi_t* creerFourmi(int fd, int type);

/* Stats ouvriere (exemple):
 pv_max = 30;
 capacite_stockage = 20;
 pts_attaque = 5;

 Stats soldate :
 pv_max = 50;
 capacite_stockage = 0;
 pts_attaque = 10; */


/**
 * Fonction de déplacement d'une ouvriere. Fonction appelée au lancement d'un thread
 * @param args coordonnées de la fourmi
 * L'algo est décrit dans le fichier "algo_ouvriere" (drive)
 */
void* deplacementOuvriere(void* args);

/**
 * Fonction de déplacement d'une soldate. Fonction appelée au lancement d'un thread
 * @param args coordonnées de la fourmi
 * L'algo est décrit dans le fichier "algo_soldate" (drive)
 */
void* deplacementSoldate(void* args);


/**
 * Fonction permettant à la fourmi de consommer de la nourriture pour regenerer des PV
 * @param nourriture_t structure nourriture
 */
void manger(fourmi_t* fourmi, nourriture_t* nourriture);

/**
 * Fonction permettant à une fourmi de transporter de la nourriture
 * La fourmi charge un maximum de nourriture (jusqu'à ce qu'il n'y ait plus de nourriture ou qu'elle ne puisse plus en transporter plus)
 * @param nourriture_t structure nourriture
 */
void chargerNourriture(fourmi_t* fourmi, nourriture_t* nourriture);

/**
 * Attaque une bibite
 * Reviens à un simple jet de dés en fonction de l'attaque de la fourmi
 * @param ennemi fourmi étrangère ou bibitte
 * On définit la nature de l'ennemi en fonction de la taille de structure passée en paramètre
 */
void attaquer(fourmi_t* fourmi, void* ennemi);

/* Transforme une fourmi en source de nourriture */
void mourrir(fourmi_t* fourmi);

/* Detruit la fourmi (ne génère pas de nourriture)
 Utilisé lors du passage d'une fourmi d'une zone A à une zone B
 La fourmi ne meurt pas, la zone arrête juste de la gérer */
void changerZone(fourmi_t* fourmi);

void* routineFourmi(void* args);

/* La fourmi se met en attente d'un signal
   Lorsque ce signal est recu, la fourmi meurt
   (destruction de la fourmi + fin du thread)
   Le signal à attenre dépends la zone d'origine de la fourmi (numero du signal à définir) */
void gestionnaireMort(int sig);
