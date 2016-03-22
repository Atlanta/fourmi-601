#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include "fourmi.h"
#include "zone.h"
#include "ncurses.h"

int main(int argc, char *argv[]) {
	WINDOW *fen_box_sim, *fen_box_msg, *fen_box_outils, *fen_outils;
	MEVENT event;
	int ch, i, item_actif = OBSTACLE;
	coord_t *coord;

	ncurses_initialiser();
	simulation_initialiser();

	fen_box_sim = creer_fenetre_box_sim();
	fen_sim = creer_fenetre_sim();
	fen_box_msg = creer_fenetre_box_msg();
	fen_msg = creer_fenetre_msg();
	fen_box_outils = creer_fenetre_box_outils();
	fen_outils = creer_fenetre_outils();

	mvprintw(LINES - 1, 0, "Tapez F2 pour quitter");
	wrefresh(stdscr);
	while((ch = getch()) != KEY_F(2)) {
		switch(ch) {
			case KEY_MOUSE :
				if (getmouse(&event) == OK) {
					/*wprintw(fen_msg, "Clic a la position %d %d de l'ecran\n", event.y, event.x);
					wrefresh(fen_msg);*/
					if (event.y == 32 && event.x >= 82 && event.x <= 98) {
						item_actif = OBSTACLE;
						mvwprintw(fen_outils, 0, 1, "X");
						mvwprintw(fen_outils, 1, 1, " ");
						wrefresh(fen_outils);
						wprintw(fen_msg, "Outil obstacle active\n");
						wrefresh(fen_msg);
					}
					else if (event.y == 33 && event.x >=82 && event.x <= 98) {
						item_actif = FOURMI;
						mvwprintw(fen_outils, 0, 1, " ");
						mvwprintw(fen_outils, 1, 1, "X");
						wrefresh(fen_outils);
						wprintw(fen_msg, "Outil fourmi active\n");
						wrefresh(fen_msg);
					}
					else if (event.y > 0 && event.y < NB_LIGNES_SIM + 1 && event.x > 0 && event.x < NB_COL_SIM + 1) {
						switch (item_actif) {
							case OBSTACLE :
								pthread_mutex_lock(&grille[event.y - 1][event.x - 1].mutex);
								if (grille[event.y - 1][event.x - 1].element == VIDE) {
									grille[event.y - 1][event.x - 1].element = OBSTACLE;
									mvwprintw(fen_sim, event.y - 1, event.x - 1, "#");
									wprintw(fen_msg, "Ajout d'un obstacle a la position %d %d\n", event.y - 1, event.x - 1);
								}
								wrefresh(fen_sim);
								wrefresh(fen_msg);
								pthread_mutex_unlock(&grille[event.y - 1][event.x - 1].mutex);
								break;
							case FOURMI :
								pthread_mutex_lock(&grille[event.y - 1][event.x - 1].mutex);
								if (grille[event.y - 1][event.x - 1].element == VIDE) {
									i = 0;
									while (threads_fourmis[i] != NULL && i < MAX_FOURMIS)
										i++;
									if (i < MAX_FOURMIS) {
										threads_fourmis[i] = (pthread_t *) malloc(sizeof(pthread_t));
										grille[event.y - 1][event.x - 1].element = FOURMI;
										grille[event.y - 1][event.x - 1].fourmi = threads_fourmis[i];
										coord = (coord_t *) malloc(sizeof(coord_t));
										coord->y = event.y - 1;
										coord->x = event.x - 1;
										pthread_create(threads_fourmis[i], NULL, routineFourmi, (void *) coord);
										mvwprintw(fen_sim, event.y - 1, event.x - 1, "@");
										wprintw(fen_msg, "Ajout d'une fourmi a la position %d %d\n", event.y - 1, event.x - 1);
									}
									else {
										wprintw(fen_msg, "Nombre maximum de fourmis atteint\n");
									}
								}
								wrefresh(fen_sim);
								wrefresh(fen_msg);
								pthread_mutex_unlock(&grille[event.y - 1][event.x - 1].mutex);
								break;
						}
					}
				}
			break;
		}
	}

	delwin(fen_box_sim);
	delwin(fen_sim);
	delwin(fen_box_msg);
	delwin(fen_msg);
	delwin(fen_box_outils);
	delwin(fen_outils);
	simulation_stopper();
	ncurses_stopper();

	return 0;
}
