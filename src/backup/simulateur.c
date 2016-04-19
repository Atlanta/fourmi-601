#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <ncurses.h>
#include "structures.h"
#include "ncurses_init.h"
#include "fourmis.h"


int main(int argc, char const *argv[]) {





    ncurses_initialiser();
    simulation_initialiser();

    fen_box_sim = creer_fenetre_box_sim();
    fen_sim = creer_fenetre_sim();


    mvprintw(NB_LIGNES_SIM + 1, 0, "Tapez F2 pour quitter");



	wrefresh(stdscr);
    creer_fourmis();
    creer_fourmis();

    /*for(i = 0; i < MAX_FOURMIS; i++)
    {
        creer_fourmis();
    }*/

    /*wrefresh(fen_sim);*/


    while(getch() != KEY_F(2)) {

	}

    delwin(fen_box_sim);
    delwin(fen_sim);
    simulation_stopper();
	ncurses_stopper();




    return 0;
}
