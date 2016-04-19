
#define _XOPEN_SOURCE
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <ncurses.h>
#include <sys/stat.h>
#include "afficheur.h"
#include "ncurses_init.h"
#include "../header/semaphores.h"
#include "../header/memoirePartagee.h"

void afficherTerrain(segment_t* terrain, WINDOW* fenetre) {
	size_t i, j;

	for(i=0; i<(size_t)*terrain->hauteur; i++) {
		for(j=0; j<(size_t)*terrain->largeur; j++) {
			switch ((int)terrain->cases[i*(int)(*terrain->hauteur)+j]) {
				case VIDE:
					mvwprintw(fenetre, i, j, " ");
					break;
				case FOURMILIERE:
					mvwprintw(fenetre, i, j, "#");
					break;
				case FOURMI:
					mvwprintw(fenetre, i, j, "@");
					break;
				case BIBITE:
					mvwprintw(fenetre, i, j, "*");
					break;
			}
		}
	}

	wrefresh(fenetre);
}

int main(int argc, char const *argv[]) {
    int ch, semid;
    key_t cle;
    void* adresse;
    WINDOW *fen_afficheur;
    WINDOW *fen_box_afficheur;
    segment_t* terrain;


    if(argc != 2){
        fprintf(stderr, "Entrer la cle pour le tableau de semaphores et le segment de memoire partagee\n");
        exit(EXIT_FAILURE);
    }
    cle = (key_t)atoi(argv[1]);


    /* Creation du terrain et du segment de memoire partagee */
    terrain = creerTerrain(cle, NB_LIGNES_AFF, NB_COL_AFF);

    /* Création et initialisation des sémaphores */
	if((semid = semget(cle, 2, S_IRUSR | S_IWUSR | IPC_CREAT | IPC_EXCL)) == -1) {
    	if(errno == EEXIST)
	        fprintf(stderr, "Tableau de semaphores (cle=%d) existant\n", cle);
    	else
        	perror("Erreur lors de la creation du tableau de semaphores ");
    	exit(EXIT_FAILURE);
    }

	init(semid, 0, 0);
	init(semid, 1, 1);


    ncurses_initialiser();

    fen_box_afficheur = creer_fenetre_box_afficheur();
    fen_afficheur = creer_fenetre_afficheur();

    afficherTerrain(terrain, fen_afficheur);


    mvprintw(NB_LIGNES_AFF + 1, 0, "Tapez F2 pour quitter");
    



    wrefresh(stdscr);

    while((ch = getch()) != KEY_F(2)){
        P(semid, 0, 1); /* Le programme d'affichage attend que le calculateur incrémente le sémaphore 0 */
		afficherTerrain(terrain, fen_afficheur); /* Met à jour l'affichage */
		V(semid, 1, 1); /* Puis incrémente le sémaphore 1 */
    }

    delwin(fen_afficheur);
    delwin(fen_box_afficheur);

    ncurses_stopper();
    destroy(semid);
    detruireTerrain(cle);
	exit(EXIT_SUCCESS);


    return 0;
}
