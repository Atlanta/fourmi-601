#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "editeur.h"

void initialiserFenetres() {
	mvprintw(LINES-1, 0, "Appuyez sur F2 pour quitter.");

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

	mvwprintw(outil, 0, 4, "Outil\tHotkey");
	mvwprintw(outil, 3, 4, "  Vide\t0/V");
	mvwprintw(outil, 5, 4, "@ Fourmi\t1/F");
	mvwprintw(outil, 7, 4, "# Obstacle\t2/O");
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

void chargerFichier(int fd) {
	int i;
	char c;
	char *vide = ICONE_VIDE, *fourmi = ICONE_FOURMI, *obstacle = ICONE_OBSTACLE;
	for (i = 0; i < HAUTEUR*LARGEUR; i++) {
		if(read(fd, &c, 1) == -1) {
			perror("Erreur d'écriture dans le fichier ");
			exit(EXIT_FAILURE);
		}
		switch (c) {
			case VIDE:
				mvwprintw(editeur, i/LARGEUR, i%LARGEUR, vide);
				break;
			case FOURMI:
				mvwprintw(editeur, i/LARGEUR, i%LARGEUR, fourmi);
				break;
		case OBSTACLE:
				mvwprintw(editeur, i/LARGEUR, i%LARGEUR, obstacle);
				break;
		}
		/*lseek(fd, 1, SEEK_CUR);*/
	}
}

void changerOutil(char numeroOutil) {
	int i;
	/* On efface la colonne */
	for (i = 0; i < HAUTEUROUTIL; i++) {
		mvwprintw(outil, i, 2, " ");
	}
	switch (numeroOutil) {
		case VIDE:
			mvwaddch(outil, 3, 2, ACS_DIAMOND);
			break;
		case FOURMI:
			mvwaddch(outil, 5, 2, ACS_DIAMOND);
			break;
		case OBSTACLE:
			mvwaddch(outil, 7, 2, ACS_DIAMOND);
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

int getFilePosition(souris_t s) {
	return (s.y-1) * LARGEUR + (s.x-1);
}

int isInZoneWindow(souris_t s) {
	if (s.x >= 1 && s.x <= LARGEUR && s.y >= 1 && s.y <= HAUTEUR)
		return 1;
	return 0;
}

void terminer() {
	ncurses_stopper();
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
	int ch, fd, position;
	char valeur[3] = {0, 1, 2};
	char *vide = ICONE_VIDE, *fourmi = ICONE_FOURMI, *obstacle = ICONE_OBSTACLE;
	souris_t curseur;

	if (argc != 2) {
		fprintf(stderr, "La commande est de la forme : %s <nom_fichier>\n", argv[0]);
		fprintf(stderr, "Où :\n");
		fprintf(stderr, "\t<nom_fichier> : Le nom du fichier de zone à créer ou à charger\n");
		exit(EXIT_FAILURE);
	}

	ncurses_initialiser();
	ncurses_souris();

	initialiserFenetres();

	refreshAll();

	if ((fd = open(argv[1], O_RDWR | O_CREAT | O_EXCL, 0755)) == -1) {
		if (errno == EEXIST) {
			if ((fd = open(argv[1], O_RDWR)) == -1) {
				perror("Erreur d'ouverture du fichier existant ");
				terminer();
			}
			wprintw(infos, "Fichier trouvé, chargement du fichier...\n");
			wrefresh(infos);
			chargerFichier(fd);
			wprintw(infos, "Chargement terminé.\n");
			wrefresh(infos);
		}
		else {
			perror("Erreur de création du fichier ");
			terminer();
		}
	}

	lseek(fd, 0, SEEK_END);
	if (lseek(fd, 0, SEEK_CUR) == 0) { /* Si le fichier est vide, on l'initialise */
		wprintw(infos, "Fichier inexistant, création du fichier...\n");
		wrefresh(infos);
		initialiserFichier(fd);
		wprintw(infos, "Création terminée.\n");
		wrefresh(infos);
	}
	lseek(fd, 0, SEEK_SET);

	changerOutil(VIDE);

	refreshAll();

	while((ch = getch()) != KEY_F(2)) {
		if(ch == KEY_MOUSE) {
			souris_getpos(&curseur);

			if (isInZoneWindow(curseur)) {
				position = lseek(fd, getFilePosition(curseur), SEEK_SET);
				switch (outilActuel) {
					case VIDE:
						if (write(fd, &valeur[VIDE], sizeof(char)) == -1) {
							perror("Erreur d'écriture dans le fichier ");
							terminer();
						}
						mvwprintw(editeur, curseur.y-1, curseur.x-1, vide);
						break;
					case FOURMI:
						if (write(fd, &valeur[FOURMI], sizeof(char)) == -1) {
							perror("Erreur d'écriture dans le fichier ");
							terminer();
						}
						mvwprintw(editeur, curseur.y-1, curseur.x-1, fourmi);
						break;
					case OBSTACLE:
						if (write(fd, &valeur[OBSTACLE], sizeof(char)) == -1) {
							perror("Erreur d'écriture dans le fichier ");
							terminer();
						}
						mvwprintw(editeur, curseur.y-1, curseur.x-1, obstacle);
						break;
				}
			}
		}
		else if (ch == '0' || ch == 'V') {
			changerOutil(VIDE);
			wprintw(infos, "Outil vide sélectionné\n");
		}
		else if (ch == '1' || ch == 'F') {
			changerOutil(FOURMI);
			wprintw(infos, "Outil fourmi sélectionné\n");
		}
		else if (ch == '2' || ch == 'O') {
			changerOutil(OBSTACLE);
			wprintw(infos, "Outil obstacle sélectionné\n");
		}

		refreshAll();
	}

	if (close(fd) == -1) {
		perror("Erreur lors de la fermeture du fichier ");
		terminer();
	}

	/* Arrêt de ncurses */
	ncurses_stopper();


	exit(EXIT_SUCCESS);
}
