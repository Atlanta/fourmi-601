#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "editeur.h"

void initialiserFichier(int fd) {
	int i;
	char c = '0';
	lseek(fd, 0, SEEK_SET);
	for (i = 0; i < HAUTEUR*LARGEUR; i++) {
		if(write(fd, &c, 1) == -1) {
			perror("Erreur d'écriture dans le fichier ");
			exit(EXIT_FAILURE);
		}
	}
}

void chargerFichier(int fd) {}

void changerOutil(int numeroOutil) {}

void refreshAll() {
	wrefresh(cadreEditeur);
	wrefresh(cadreInfos);
	wrefresh(editeur);
	wrefresh(infos);
	refresh();
}

int getFilePosition(souris_t s) {
	return (s.x - 1) * (s.y - 1);
}

int isInZoneWindow(souris_t s) {
	if (s.x >= 1 && s.x <= LARGEUR && s.y >= 1 && s.y <= HAUTEUR)
		return 1;
	return 0;
}

int main(int argc, char* argv[]) {
	int ch, fd, position, outilActuel = 0;
	char vide = ' ', fourmi = ICONE_FOURMI, obstacle = ICONE_OBSTACLE;
	souris_t curseur;

	if (argc != 2) {
		fprintf(stderr, "La commande est de la forme : %s <nom_fichier>\n", argv[0]);
		fprintf(stderr, "Où :\n");
		fprintf(stderr, "\t<nom_fichier> : Le nom du fichier de zone à créer ou à charger\n");
		exit(EXIT_FAILURE);
	}

	if ((fd = open(argv[1], O_RDWR | O_CREAT | O_EXCL, 0755)) == -1) {
		if (errno == EEXIST) {
			if ((fd = open(argv[1], O_RDWR)) == -1) {
				perror("Erreur d'ouverture du fichier existant ");
				exit(EXIT_FAILURE);
			}
			chargerFichier(fd);
		}
		else {
			perror("Erreur de création du fichier ");
			exit(EXIT_FAILURE);
		}
	}

	lseek(fd, 0, SEEK_END);
	if (lseek(fd, 0, SEEK_CUR) != 0) /* Si le fichier est vide, on l'initialise */
		initialiserFichier(fd);
	lseek(fd, 0, SEEK_SET);

	ncurses_initialiser();
	ncurses_souris();

	mvprintw(LINES-1, 0, "Appuyez sur F2 pour quitter.");

	cadreEditeur = newwin(HAUTEUR+2, LARGEUR+2, 0, 0);
	cadreInfos = newwin(HAUTEURINFO+2, LARGEUR+2, 0, LARGEUR+2);
	box(cadreEditeur, ACS_VLINE, ACS_HLINE);
	box(cadreInfos, ACS_VLINE, ACS_HLINE);
	wprintw(cadreEditeur, "Zone");
	wprintw(cadreInfos, "Informations");

	editeur = newwin(HAUTEUR, LARGEUR, 1, 1);
	infos = newwin(HAUTEURINFO, LARGEURINFO, 1, LARGEUR+3);

	refreshAll();

	while((ch = getch()) != KEY_F(2)) {
		if(ch == KEY_MOUSE) {
			souris_getpos(&curseur);

			if (isInZoneWindow(curseur)) {
				position = lseek(fd, getFilePosition(curseur), SEEK_SET);
				switch (outilActuel) {
					case VIDE:
						if (write(fd, &vide, position) == -1) {
							perror("Erreur d'écriture dans le fichier ");
							exit(EXIT_FAILURE);
						}
						mvwprintw(editeur, curseur.y, curseur.x, &vide);
						break;
					case FOURMI:
						if (write(fd, &fourmi, position) == -1) {
							perror("Erreur d'écriture dans le fichier ");
							exit(EXIT_FAILURE);
						}
						mvwprintw(editeur, curseur.y, curseur.x, &fourmi);
						break;
					case OBSTACLE:
						if (write(fd, &obstacle, position) == -1) {
							perror("Erreur d'écriture dans le fichier ");
							exit(EXIT_FAILURE);
						}
						mvwprintw(editeur, curseur.y, curseur.x, &obstacle);
						break;
				}
			}
		}
		else if (ch == '0') {
			outilActuel = 0;
		}
		else if (ch == '1') {
			outilActuel = 1;
		}
		else if (ch == '2') {
			outilActuel = 2;
		}

		refreshAll();
	}

	/* Arrêt de ncurses */
	ncurses_stopper();

	/* TODO Enregistrement dans un fichier */
	if (close(fd) == -1) {
		perror("Erreur lors de la fermeture du fichier ");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
