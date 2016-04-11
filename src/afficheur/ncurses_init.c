#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include "ncurses_init.h"

void ncurses_initialiser() {
	initscr();								/* Demarre le mode ncurses */
	cbreak();								/* Pour les saisies clavier (desac. mise en buffer) */
	noecho();								/* Desactive l'affichage des caracteres saisis */
	keypad(stdscr, TRUE);					/* Active les touches specifiques */
	refresh();								/* Met a jour l'affichage */
	curs_set(FALSE);						/* Masque le curseur */
	mousemask(BUTTON1_CLICKED, NULL);		/* Active le clic gauche de la souris*/
}

void ncurses_stopper() {
	endwin();
}



WINDOW *creer_fenetre_box_afficheur() {

	WINDOW *fen_box_afficheur;

	fen_box_afficheur = newwin(NB_LIGNES_AFF + 2, NB_COL_AFF + 2, 0, 0);
	box(fen_box_afficheur, 0, 0);
	mvwprintw(fen_box_afficheur, 0, (NB_COL_AFF + 2) / 2 - 5, "AFFICHEUR");
	wrefresh(fen_box_afficheur);

	return fen_box_afficheur;
}

WINDOW *creer_fenetre_afficheur() {


	WINDOW *fen_afficheur;

	fen_afficheur = newwin(NB_LIGNES_AFF, NB_COL_AFF, 1, 1);


	return fen_afficheur;
}
