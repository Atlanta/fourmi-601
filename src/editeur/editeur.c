#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>

#include "editeur.h"

void handler(int sig) {
	char ch = 0, nomFichier[256];
	if(sig == SIGINT) {
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

				enregistrerFichier(nomFichier);

				ncurses_stopper();
				exit(EXIT_FAILURE);
			}
			else {
				ncurses_stopper();
				exit(EXIT_FAILURE);
			}
		}
	}
}

void drawBottomBar() {
	int i;

	for (i = 0; i < COLS; i++) {
		mvaddch(LINES-1, i, ' ');
	}
	attron(COLOR_PAIR(4));
	mvprintw(LINES-1, 0, "F2");
	mvprintw(LINES-1, 11, "F8");
	mvprintw(LINES-1, 26, "F10");
	attroff(COLOR_PAIR(4));

	mvprintw(LINES-1, 3, "Charger");
	mvprintw(LINES-1, 14, "Enregistrer");
	mvprintw(LINES-1, 30, "Quitter");

	refresh();
}

void eraseBottomBar() {
	int i;

	attron(COLOR_PAIR(4));
	for (i = 0; i < COLS; i++) {
		mvaddch(LINES-1, i, ' ');
	}
	attroff(COLOR_PAIR(4));

	refresh();
}

void initialiserFenetres() {
	drawBottomBar();

	cadreEditeur = newwin(HAUTEUR+2, LARGEUR+2, 0, 0);
	cadreInfos = newwin(HAUTEURINFO+2, LARGEURINFO+2, 0, LARGEUR+2);
	cadreOutil = newwin(HAUTEUROUTIL+2, LARGEUROUTIL+2, HAUTEURINFO+2, LARGEUR+2);
	box(cadreEditeur, ACS_VLINE, ACS_HLINE);
	box(cadreInfos, ACS_VLINE, ACS_HLINE);
	box(cadreOutil, ACS_VLINE, ACS_HLINE);
	wprintw(cadreEditeur, "Zone");
	wprintw(cadreInfos, "Informations");
	wprintw(cadreOutil, "Outils");

	editeur = newwin(HAUTEUR, LARGEUR, 1, 1);
	infos = newwin(HAUTEURINFO, LARGEURINFO, 1, LARGEUR+3);
	outil = newwin(HAUTEUROUTIL, LARGEUROUTIL, HAUTEURINFO+3, LARGEUR+3);

	scrollok(infos, true);

	mvwprintw(outil, 0, 4, "Outil\tHotkey");
	mvwaddch(outil, 3, 4, ICONE_VIDE);
	mvwprintw(outil, 3, 5, " Vide\t0/V");
	mvwaddch(outil, 5, 4, ICONE_FOURMILIERE);
	mvwprintw(outil, 5, 5, " Fourmil.\t1/F");
	mvwaddch(outil, 7, 4, ICONE_OBSTACLE);
	mvwprintw(outil, 7, 5, " Obstacle\t2/O");
}

void initialiserFichier(int fd) {
	int i;
	char c = 0;
	lseek(fd, 0, SEEK_SET);
	for (i = 0; i < HAUTEUR*LARGEUR; i++) {
		if(write(fd, &c, 1) == -1) {
			perror("Erreur d'écriture dans le fichier ");
			exit(EXIT_FAILURE);
		}
	}
}

void chargerFichier(char* nomFichier) {
	int fd, i;
	char c;

	if ((fd = open(nomFichier, O_RDWR)) == -1) {
		if (errno == ENOENT) {
			wprintw(infos, "Erreur lors du chargement du fichier : le fichier n'existe pas.\n");
			return;
		}
		else {
			wprintw(infos, "Erreur lors du chargement du fichier, veuillez réessayer.\n");
			return;
		}
	}

	for (i = 0; i < HAUTEUR*LARGEUR; i++) {
		if(read(fd, &c, 1) == -1) {
			wprintw(infos, "Erreur lors de l'écriture dans le fichier, veuillez réessayer.\n");
			return;
		}
		switch (c) {
		case VIDE:
			mvwaddch(editeur, i/LARGEUR, i%LARGEUR, ICONE_VIDE);
			break;
		case FOURMILIERE:
			mvwaddch(editeur, i/LARGEUR, i%LARGEUR, ICONE_FOURMILIERE);
			break;
		case OBSTACLE:
			mvwaddch(editeur, i/LARGEUR, i%LARGEUR, ICONE_OBSTACLE);
			break;
		}
	}

	if (close(fd) == -1) {
		wprintw(infos, "Erreur lors de la fermeture du fichier.\n");
		return;
	}

	wprintw(infos, "Fichier chargé.\n");
}

void changerOutil(char numeroOutil) {
	int i;
	/* On efface la colonne */
	for (i = 0; i < HAUTEUROUTIL; i++) {
		mvwprintw(outil, i, 2, " ");
	}
	switch (numeroOutil) {
	case VIDE:
		mvwaddch(outil, 3, 2, ACS_RARROW);
		break;
	case FOURMILIERE:
		mvwaddch(outil, 5, 2, ACS_RARROW);
		break;
	case OBSTACLE:
		mvwaddch(outil, 7, 2, ACS_RARROW);
		break;
	}
	outilActuel = numeroOutil;
}

void refreshAll() {
	wrefresh(cadreEditeur);
	wrefresh(cadreInfos);
	wrefresh(cadreOutil);
	wrefresh(editeur);
	wrefresh(infos);
	wrefresh(outil);
	refresh();
}

int getPosition(souris_t s) {
	return (s.y-1) * LARGEUR + (s.x-1);
}

int isInZoneWindow(souris_t s) {
	if (s.x >= 1 && s.x <= LARGEUR && s.y >= 1 && s.y <= HAUTEUR)
		return 1;
	return 0;
}

void ecrireFichier(int fd) {
	int i;

	for (i = 0; i < HAUTEUR*LARGEUR; i++) {
		if (write(fd, &grille[i], sizeof(char)) == -1) {
			wprintw(infos, "Erreur lors de l'écriture dans le fichier, veuillez réessayer.\n");
			return;
		}
	}
}

void enregistrerFichier(char* nomFichier) {
	int fd;
	char ch;

	if(strlen(nomFichier) == 0) {
		wprintw(infos, "Le nom du fichier ne peut pas être vide !\n");
		return;
	}

	if ((fd = open(nomFichier, O_RDWR | O_CREAT | O_EXCL, 0755)) == -1) {
		if (errno == EEXIST) {
			if ((fd = open(nomFichier, O_RDWR)) == -1) {
				wprintw(infos, "Erreur lors de l'ouverture du fichier, veuillez réessayer.\n");
				return;
			}
			eraseBottomBar();
			attron(COLOR_PAIR(4));
			mvprintw(LINES-1, 0, "Le fichier existe déjà, voulez-vous le remplacer ? (O/N) ");
			attroff(COLOR_PAIR(4));
			refresh();
			while(1) {
				ch = getch();

				if (ch == 'O' || ch == 'o') {
					ecrireFichier(fd);
					wprintw(infos, "Fichier enregistré.\n");
					return;
				}
				else {
					return;
				}
			}
		}
		else {
			wprintw(infos, "Erreur lors de la création du fichier, veuillez réessayer.\n");
			return;
		}
	}

	ecrireFichier(fd);

	if (close(fd) == -1) {
		wprintw(infos, "Erreur lors de l'enregistrement du fichier, veuillez réessayer.\n");
		return;
	}

	wprintw(infos, "Fichier enregistré.\n");
}

/* TODO Fonction de fin perso avec paramètre pour code exit */
void terminer(int code) {
	ncurses_stopper();
	exit(code);
}

int main(int argc, char* argv[]) {
	int position, ch;
	char nomFichier[256] = "";
	souris_t curseur;
	struct sigaction sig;

	nbTypesFourmi = 0;
	nbBibites = 0;
	periodiciteNourriture = 1;

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

	refreshAll();

	memset(grille, 0, HAUTEUR*LARGEUR);
	changerOutil(VIDE);

	if (argc == 2)
		chargerFichier(argv[1]);

	refreshAll();

	while((ch = getch()) != KEY_F(10)) {
		if(ch == KEY_MOUSE) {
			souris_getpos(&curseur);

			if (isInZoneWindow(curseur)) {
				position = getPosition(curseur);
				grille[position] = outilActuel;
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
		else if (ch == '0' || ch == 'V') {
			changerOutil(VIDE);
			wprintw(infos, "Outil vide sélectionné\n");
		}
		else if (ch == '1' || ch == 'F') {
			changerOutil(FOURMILIERE);
			wprintw(infos, "Outil fourmi sélectionné\n");
		}
		else if (ch == '2' || ch == 'O') {
			changerOutil(OBSTACLE);
			wprintw(infos, "Outil obstacle sélectionné\n");
		}
		else if (ch == KEY_F(2)) {
			eraseBottomBar();
			attron(COLOR_PAIR(4));
			mvprintw(LINES-1, 0, "Nom du fichier : ");
			writing_mode(true);
			mvscanw(LINES-1, 17, "%255s", nomFichier);
			writing_mode(false);
			attroff(COLOR_PAIR(4));
			chargerFichier(nomFichier);
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
			enregistrerFichier(nomFichier);
			drawBottomBar();
		}
		else if (ch == KEY_F(10)) {
			kill(getpid(), SIGINT);
		}

		refreshAll();
	}

	ncurses_stopper();
	exit(EXIT_SUCCESS);
}
