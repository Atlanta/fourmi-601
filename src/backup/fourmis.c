#include "fourmis.h"
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include "structures.h"
#include "ncurses_init.h"
#include <unistd.h>
#include <stdio.h>



void creer_fourmis()
{
    int x, y, i;
    coord_t* coord;

    i = 0;
    while (threads_fourmis[i] != NULL && i < MAX_FOURMIS){
        i++;
    }

    srand(time(NULL));
    if(i < MAX_FOURMIS){
        threads_fourmis[i] = (pthread_t*) malloc(sizeof(pthread_t));
        do {
            x = rand() % NB_COL_SIM;
            y = rand() % NB_LIGNES_SIM;
        } while(grille[y][x].element != VIDE);


        grille[y][x].element = FOURMI;
        grille[y][x].fourmi = threads_fourmis[i];


        coord = (coord_t *) malloc(sizeof(coord_t));

        coord->y = y;
        coord->x = x;
        pthread_create(threads_fourmis[i], NULL, routine_fourmi, (void*)coord);

        mvwprintw(fen_sim, y, x, "@");
        wrefresh(fen_sim);
    }

}



void *routine_fourmi(void *arg){
    coord_t *coord = (coord_t *) arg;
	srand(time(NULL));


	while (1) {
		switch(rand()%4){
			case 0:
				if(coord->x - 1> 0)
				{
					if(grille[coord->y][coord->x-1].element != FOURMI)
					{
                        grille[coord->y][coord->x].element = VIDE;
						mvwprintw(fen_sim, coord->y, coord->x, " ");
						coord->x--;
						grille[coord->y][coord->x].element = FOURMI;
						mvwprintw(fen_sim, coord->y, coord->x, "@");
						wrefresh(fen_sim);
					}
				}

				break;
			case 1:
				if(coord->x + 1< NB_COL_SIM)
				{
					if(grille[coord->y][coord->x+1].element != FOURMI)
					{
                        grille[coord->y][coord->x].element = VIDE;
						mvwprintw(fen_sim, coord->y, coord->x, " ");
						coord->x++;
						grille[coord->y][coord->x].element = FOURMI;
						mvwprintw(fen_sim, coord->y, coord->x, "@");
						wrefresh(fen_sim);
					}
				}
				break;
			case 2:
				if(coord->y - 1 > 0)
				{
					if(grille[coord->y -1][coord->x].element != FOURMI)
					{
                        grille[coord->y][coord->x].element = VIDE;
						mvwprintw(fen_sim, coord->y, coord->x, " ");
						coord->y--;
						grille[coord->y][coord->x].element = FOURMI;
						mvwprintw(fen_sim, coord->y, coord->x, "@");
						wrefresh(fen_sim);
					}
				}
				break;
			case 3:
				if(coord->y + 1 < NB_LIGNES_SIM)
				{
					if(grille[coord->y + 1][coord->x].element != FOURMI)
					{
                        grille[coord->y][coord->x].element = VIDE;
						mvwprintw(fen_sim, coord->y, coord->x, " ");
						coord->y++;
						grille[coord->y][coord->x].element = FOURMI;
						mvwprintw(fen_sim, coord->y, coord->x, "@");
						wrefresh(fen_sim);
					}


				}

				break;
		}

		sleep(1);
	}

	free(coord);
	return NULL;
}
