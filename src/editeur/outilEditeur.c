#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>

#include "outilEditeur.h"

void handler(int sig) {
	if(sig == SIGINT) {
		ncurses_stopper();
		exit(EXIT_FAILURE);
	}
}

void drawBottomBar() {
	int i;

	for (i = 0; i < COLS; i++) {
		mvaddch(LINES-1, i, ' ');
	}
	attron(COLOR_PAIR(4));
	mvprintw(LINES-2, 0, "T");
	mvprintw(LINES-1, 0, "F2");
	mvprintw(LINES-1, 11, "F8");
	mvprintw(LINES-1, 26, "F10");
	attroff(COLOR_PAIR(4));

	mvprintw(LINES-2, 2, "Config. types fourmi");
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

void dessinerFenetreZone() {
	
}
void dessinerFenetreInfos();
void dessinerFenetreOutils();
void dessinerFenetreConfig();

void initialiserFenetres() {
	int i;

	drawBottomBar();

	cadreEditeur = newwin(HAUTEUR+2, LARGEUR+2, 0, 0);
	cadreInfos = newwin(HAUTEUR_INFO+2, LARGEUR_INFO+2, 0, LARGEUR+2);
	cadreOutil = newwin(HAUTEUR_OUTIL+2, LARGEUR_OUTIL+2, HAUTEUR_INFO+2, LARGEUR+2);
	cadreConfig = newwin(HAUTEUR_CONFIG+2, LARGEUR_CONFIG+2, HAUTEUR_INFO+2+HAUTEUR_OUTIL+2, LARGEUR+2);
	box(cadreEditeur, ACS_VLINE, ACS_HLINE);
	box(cadreInfos, ACS_VLINE, ACS_HLINE);
	box(cadreOutil, ACS_VLINE, ACS_HLINE);
	box(cadreConfig, ACS_VLINE, ACS_HLINE);
	wprintw(cadreEditeur, "Zone");
	wprintw(cadreInfos, "Informations");
	wprintw(cadreOutil, "Outils");
	wprintw(cadreConfig, "Configuration");

	editeur = newwin(HAUTEUR, LARGEUR, 1, 1);
	infos = newwin(HAUTEUR_INFO, LARGEUR_INFO, 1, LARGEUR+3);
	outil = newwin(HAUTEUR_OUTIL, LARGEUR_OUTIL, HAUTEUR_INFO+3, LARGEUR+3);
	config = newwin(HAUTEUR_CONFIG, LARGEUR_CONFIG, HAUTEUR_INFO+HAUTEUR_OUTIL+5, LARGEUR+3);

	scrollok(infos, true);

	mvwaddch(outil, 2, 4, ICONE_VIDE);
	mvwaddch(outil, 3, 4, ICONE_FOURMILIERE);
	mvwaddch(outil, 4, 4, ICONE_OBSTACLE);
	mvwprintw(outil, 0, 4, "Outil\tHotkey");
	mvwprintw(outil, 2, 6, "Vide\t0/V");
	mvwprintw(outil, 3, 6, "Fourmil.\t1/F");
	mvwprintw(outil, 4, 6, "Obstacle\t2/O");

	mvwprintw(config, 0, 0, "Paramètre\t\tValeur");
	mvwprintw(config, 2, 0, "Types de fourmi\t\t%d", 0);
	mvwprintw(config, 3, 0, "Nb. de bibites\t\t%d", 0);
	mvwprintw(config, 4, 0, "Tps. nourriture\t\t%ds", 1);
	mvwprintw(config, 5, 0, "Nb. de sources\t\t%d", 0);
	mvwprintw(config, 6, 0, "Qté. nourriture\t\t%d", 0);
	for (i = 0; i < LARGEUR_CONFIG; i++) {
		mvwaddch(config, 1, i, ACS_HLINE);
	}
	for (i = 0; i < HAUTEUR_CONFIG; i++) {
		mvwaddch(config, i, 20, ACS_VLINE);
	}
}

int chargerFichier(char* nomFichier, fichier_zone_t* zone) {
	int fd, i;
	unsigned char c;

	if ((fd = open(nomFichier, O_RDWR)) == -1) {
		if (errno == ENOENT) {
			wprintw(infos, "Erreur lors du chargement du fichier :\n");
			wprintw(infos, ERREUR);
			wprintw(infos, "\n");
			return EXIT_FAILURE;
		}
	}

	for (i = 0; i < HAUTEUR*LARGEUR; i++) {
		if(read(fd, &c, sizeof(unsigned char)) == -1) {
			wprintw(infos, "Erreur lors de l'écriture dans le fichier :\n");
			wprintw(infos, ERREUR);
			wprintw(infos, "\n");
			return EXIT_FAILURE;
		}
		switch (c) {
		case VIDE:
			mvwaddch(editeur, i/LARGEUR, i%LARGEUR, ICONE_VIDE);
			break;
		case FOURMILIERE:
			zone->grille[i] = FOURMILIERE;
			mvwaddch(editeur, i/LARGEUR, i%LARGEUR, ICONE_FOURMILIERE);
			break;
		case OBSTACLE:
			zone->grille[i] = OBSTACLE;
			mvwaddch(editeur, i/LARGEUR, i%LARGEUR, ICONE_OBSTACLE);
			break;
		}
	}

	if (close(fd) == -1) {
		wprintw(infos, "Erreur lors de la fermeture du fichier :\n");
		wprintw(infos, ERREUR);
		wprintw(infos, "\n");
		return EXIT_FAILURE;
	}

	wprintw(infos, "Fichier chargé.\n");
	return EXIT_SUCCESS;
}

void changerOutil(char numeroOutil, char* outilActuel) {
	int i;
	/* On efface la colonne */
	for (i = 0; i < HAUTEUR_OUTIL; i++) {
		mvwprintw(outil, i, 2, " ");
	}
	switch (numeroOutil) {
	case VIDE:
		mvwaddch(outil, 2, 2, ACS_RARROW);
		break;
	case FOURMILIERE:
		mvwaddch(outil, 3, 2, ACS_RARROW);
		break;
	case OBSTACLE:
		mvwaddch(outil, 4, 2, ACS_RARROW);
		break;
	}
	*outilActuel = numeroOutil;
}

void refreshAll() {
	wrefresh(cadreEditeur);
	wrefresh(cadreInfos);
	wrefresh(cadreOutil);
	wrefresh(cadreConfig);
	wrefresh(editeur);
	wrefresh(infos);
	wrefresh(outil);
	wrefresh(config);
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

void configurerTypeFourmi(fichier_zone_t* zone) {
	int ch = 0;
	int nbTypesFourmi = zone->nbTypesFourmi;
	werase(config);

	mvwprintw(config, 1, 1, "%d. Ajouter un type", AJOUTER);
	mvwprintw(config, 3, 1, "%d. Modifier un type", MODIFIER);

	while(1) {
		ch = getch();
		switch (ch) {
			case AJOUTER:
				werase(config);

				break;
		}
	}
}

int ecrireFichier(int fd, fichier_zone_t* zone) {
	int i;

	for (i = 0; i < HAUTEUR*LARGEUR; i++) {
		if (write(fd, &zone->grille[i], sizeof(char)) == -1) {
			wprintw(infos, "Erreur lors de l'écriture dans le fichier : %s\n", ERREUR);
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

int enregistrerFichier(char* nomFichier, fichier_zone_t* zone) {
	int fd;
	char ch;

	if(strlen(nomFichier) == 0) {
		wprintw(infos, "Le nom du fichier ne peut pas être vide !\n");
		return EXIT_FAILURE;
	}

	if ((fd = open(nomFichier, O_RDWR | O_CREAT | O_EXCL, 0755)) == -1) {
		if (errno == EEXIST) {
			if ((fd = open(nomFichier, O_RDWR)) == -1) {
				wprintw(infos, "Erreur lors de l'enregistrement du fichier : ");
				wprintw(infos, ERREUR);
				wprintw(infos, "\n");
				return EXIT_FAILURE;
			}
			eraseBottomBar();
			attron(COLOR_PAIR(4));
			mvprintw(LINES-1, 0, "Le fichier existe déjà, voulez-vous le remplacer ? (O/N) ");
			attroff(COLOR_PAIR(4));
			refresh();

			while(1) {
				ch = getch();

				if (ch == 'O' || ch == 'o') {
					ecrireFichier(fd, zone);
					wprintw(infos, "Fichier enregistré.\n");
					return EXIT_SUCCESS;
				}
				else
					return EXIT_SUCCESS;
			}
		}
		else {
			wprintw(infos, "Erreur lors de la création du fichier : ");
			wprintw(infos, ERREUR);
			wprintw(infos, "\n");
			return EXIT_FAILURE;
		}
	}

	ecrireFichier(fd, zone);

	if (close(fd) == -1) {
		wprintw(infos, "Erreur lors de la fermeture du fichier : ");
		wprintw(infos, ERREUR);
		wprintw(infos, "\n");
		return EXIT_FAILURE;
	}

	wprintw(infos, "Fichier enregistré.\n");
	return EXIT_SUCCESS;
}
