#ifndef __AFFICHEUR_H__
#define __AFFICHEUR_H__


#include <ncurses.h>
#include "../header/memoirePartagee.h"

#define VIDE 0
#define FOURMILIERE 1
#define FOURMI 2
#define BIBITE 3

void afficherTerrain(segment_t* terrain, WINDOW* fenetre);
#endif
