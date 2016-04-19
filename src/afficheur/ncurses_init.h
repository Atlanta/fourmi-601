#ifndef __NCURSES_INIT_H__
#define __NCURSES_INIT_H__

#include <ncurses.h>

#define NB_LIGNES_AFF 30
#define NB_COL_AFF 100



void ncurses_initialiser();
void ncurses_stopper();
WINDOW *creer_fenetre_box_afficheur();
WINDOW *creer_fenetre_afficheur();
#endif
