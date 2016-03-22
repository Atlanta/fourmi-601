#include "ncurses.h"

#include <ncurses.h>   /* Pour toutes les fonctions/constantes ncurses */
#include <stdlib.h>    /* Pour exit, EXIT_FAILURE */

/**
 * Initialisation de ncurses.
 */
void ncurses_initialiser() {
  initscr();	        /* Demarre le mode ncurses */
  /*cbreak();	         Pour les saisies clavier (desac. mise en buffer) */
  noecho();             /* Desactive l'affichage des caracteres saisis */
  keypad(stdscr, TRUE);	/* Active les touches specifiques */
  refresh();            /* Met a jour l'affichage */
  /*curs_set(FALSE);       Masque le curseur */
}

/**
 * Fin de ncurses.
 */
void ncurses_stopper() {
  endwin();
}

/**
 * Initialisation des couleurs.
 */
void ncurses_couleurs() {
  /* Verification du support de la couleur */
  if(has_colors() == FALSE) {
    ncurses_stopper();
    fprintf(stderr, "Le terminal ne supporte pas les couleurs.\n");
    exit(EXIT_FAILURE);
  }

  /* Activation des couleurs */
  start_color();

  /* Definition de la palette */
  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
}

/**
 * Initialisation de la souris.
 */
void ncurses_souris() {
  if(!mousemask(ALL_MOUSE_EVENTS, NULL)) {
    ncurses_stopper();
    fprintf(stderr, "Erreur lors de l'initialisation de la souris.\n");
    exit(EXIT_FAILURE);
  }

}

/**
 * Recupere la position x et y de la souris.
 * @param[out] x la position en x
 * @param[out] y la position en y
 * @param[out] bouton l'évenement associé au clic
 * @return OK si reussite
 */
int souris_getpos(souris_t* souris) {
  MEVENT event;
  int resultat = getmouse(&event);

  if(resultat == OK) {
    souris->x = event.x;
    souris->y = event.y;
    souris->bouton = event.bstate;
  }
  return resultat;
}
