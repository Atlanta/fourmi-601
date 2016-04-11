#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "ncurses_init.h"
#include "../header/semaphores.h"


int main(int argc, char const *argv[]) {
    int ch;
    WINDOW *fen_afficheur;
    WINDOW *fen_box_afficheur;
    ncurses_initialiser();

    fen_box_afficheur = creer_fenetre_box_afficheur();
    fen_afficheur = creer_fenetre_afficheur();



    mvprintw(NB_LIGNES_AFF + 1, 0, "Tapez F2 pour quitter");

    wrefresh(stdscr);

    while((ch = getch()) != KEY_F(2)){


    }

    delwin(fen_afficheur);
    delwin(fen_box_afficheur);

    ncurses_stopper();


    return 0;
}
