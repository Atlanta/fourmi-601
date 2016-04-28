#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>

#include "outilEditeur.h"

int main(int argc, char* argv[]) {
	int position, ch;
	char outilActuel = 0, nomFichier[256] = "";
	souris_t curseur;
	fichier_zone_t zone;
	struct sigaction sig;

	zone.nbTypesFourmi = 0;
	zone.nbBibites = 0;
	zone.gestionnaireNourriture.delay = 0;
	zone.gestionnaireNourriture.nbSources = 0;
	zone.gestionnaireNourriture.qteNourritureParSource = 0;

	sig.sa_handler = handler;
	sigaction(SIGINT, &sig, NULL);

	if (argc > 2) {
		fprintf(stderr, "La commande est de la forme : %s [<nom_fichier>]\n", argv[0]);
		fprintf(stderr, "Où :\n");
		fprintf(stderr, "\t<nom_fichier> : Le nom du fichier de zone à charger (facultatif)\n");
		exit(EXIT_FAILURE);
	}

	ncurses_initialiser();
	ncurses_souris();
	ncurses_couleurs();

	initialiserFenetres();

	memset(zone.grille, 0, HAUTEUR*LARGEUR);
	memset(zone.typesFourmi, 0, NB_TYPES_FOURMI_MAX * sizeof(type_fourmi_t));
	memset(zone.bibites, 0, NB_BIBITES_MAX * sizeof(bibite_t));
	changerOutil(VIDE, &outilActuel);

	if (argc == 2)
		chargerFichier(argv[1], &zone);

	dessinerFenetreConfig(&zone);

	refreshAll();

	while((ch = getch()) != KEY_F(10)) {
		if(ch == KEY_MOUSE) {
			souris_getpos(&curseur);

			if (isInZoneWindow(curseur)) {
				position = getPosition(curseur);
				zone.grille[position] = outilActuel;
				switch (outilActuel) {
				case VIDE:
					mvwaddch(editeur, curseur.y-1, curseur.x-1, ICONE_VIDE);
					break;
				case FOURMILIERE:
					mvwaddch(editeur, curseur.y-1, curseur.x-1, ICONE_FOURMILIERE);
					break;
				case OBSTACLE:
					mvwaddch(editeur, curseur.y-1, curseur.x-1, ICONE_OBSTACLE);
					break;
				}
			}
		}
		else if (ch == 'V' || ch == 'v') {
			changerOutil(VIDE, &outilActuel);
			wprintw(infos, "Outil vide sélectionné\n");
		}
		else if (ch == 'F' || ch == 'f') {
			changerOutil(FOURMILIERE, &outilActuel);
			wprintw(infos, "Outil fourmi sélectionné\n");
		}
		else if (ch == 'O' || ch == 'o') {
			changerOutil(OBSTACLE, &outilActuel);
			wprintw(infos, "Outil obstacle sélectionné\n");
		}
		else if (ch == 'T' || ch == 't') {
			 configurerTypeFourmi(&zone);
		}
		else if (ch == 'B' || ch == 'b') {
			 configurerBibites(&zone);
		}
		else if (ch == 'N' || ch == 'n') {
			 configurerGestionnaireNourriture(&zone);
		}
		else if (ch == 'C' || ch == 'c') {
			 configurerFourmiliere(&zone);
		}
		else if (ch == KEY_F(2)) {
			eraseBottomBar();
			attron(COLOR_PAIR(4));
			mvprintw(LINES-1, 0, "Nom du fichier : ");
			writing_mode(true);
			mvscanw(LINES-1, 17, "%255s", nomFichier);
			writing_mode(false);
			attroff(COLOR_PAIR(4));
			chargerFichier(nomFichier, &zone);
			drawBottomBar();
		}
		else if (ch == KEY_F(8)) {
			eraseBottomBar();
			attron(COLOR_PAIR(4));
			mvprintw(LINES-1, 0, "Nom du fichier : ");
			writing_mode(true);
			mvscanw(LINES-1, 17, "%255s", nomFichier);
			writing_mode(false);
			attroff(COLOR_PAIR(4));
			enregistrerFichier(nomFichier, &zone);
			drawBottomBar();
		}

		refreshAll();
	}

	eraseBottomBar();

	attron(COLOR_PAIR(4));
	mvprintw(LINES-1, 0, "Voulez-vous enregistrer avant de quitter ? (O/N)");
	attroff(COLOR_PAIR(4));
	refresh();

	while(1) {
		ch = getch();
		if (ch == 'O' || ch == 'o') {
			eraseBottomBar();

			attron(COLOR_PAIR(4));
			mvprintw(LINES-1, 0, "Nom du fichier : ");
			writing_mode(true);
			mvscanw(LINES-1, 17, "%255s", nomFichier);
			writing_mode(false);
			attroff(COLOR_PAIR(4));

			if(enregistrerFichier(nomFichier, &zone) == -1) {
				ncurses_stopper();
				fprintf(stderr, "Erreur lors de l'enregistrement du fichier\n");
				exit(EXIT_FAILURE);
			}
			else {
				ncurses_stopper();
				fprintf(stdout, "Fichier enregistré avec succès\n");
				break;
			}
		}
		else {
			ncurses_stopper();
			exit(EXIT_SUCCESS);
		}
	}

	exit(EXIT_SUCCESS);
}
