#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <ncurses.h>

#include "ncurses.h"

int asciiToHex(int key) {
	if(key >= 48 && key <= 57)
		return key-48;
	else if(key >= 97 && key <= 102)
		return key-97+10;
	else
		return -1;
}

void afficherFichier(int fd, int depart, int taille, WINDOW* fenetreHexa, WINDOW* fenetreChar) {
	unsigned int valeur;
	int i;

	lseek(fd, depart, SEEK_SET);

	werase(fenetreHexa);
	werase(fenetreChar);

	for(i = 0; i < 64; i++) {
		if(taille-(depart+i) > 0) {
			if(read(fd, &valeur, 1) == -1) {
				perror("Erreur lors de la lecture ");
				exit(EXIT_FAILURE);
			}
			wprintw(fenetreHexa, "%02x ", valeur);
			if(valeur > 32 && valeur < 127)
				wprintw(fenetreChar, "%c ", valeur);
			else
				wprintw(fenetreChar, ". ");
		}
	}

	wrefresh(fenetreHexa);
	wrefresh(fenetreChar);
}

int main(int argc, char* argv[]) {
	int depart, nboctets, fd, tmp, ch;
	int hexcols = 24;
	int charcols = 16;
	WINDOW *info, *hexa, *character, *subinfo, *subhexa, *subchar;
	souris_t curseur;
	/* Position par défaut */
	curseur.x = 1;
	curseur.y = 11;

	if(argc < 2 || argc > 3) {
		fprintf(stderr, "Mauvais arguments.\n");
		exit(EXIT_FAILURE);
	}

	if((fd = open(argv[1], O_RDWR)) == -1) {
		perror("Erreur d'ouverture du fichier source ");
		exit(EXIT_FAILURE);
	}

	nboctets = lseek(fd, 0, SEEK_END);

	if(argc == 3)
		depart = atoi(argv[2]);
	else
		depart = 0;

	/* Initialisation de ncurses */
	ncurses_initialiser();
	ncurses_souris();

	mvprintw(LINES-1, 0, "Appuyez sur F2 pour quitter.");

	/* Placement des fenêtres */
	/* Infos*/
	info = newwin(10, COLS, 0, 0);
	box(info, ACS_VLINE, ACS_HLINE);
	wprintw(info, "Informations");
	wrefresh(info);

	subinfo = newwin(8, COLS - 2, 1, 1);
	wprintw(subinfo, "Fichier : %s\n", argv[1]);
	wprintw(subinfo, "Taille : %d octets\n", nboctets);
	wprintw(subinfo, "Chargement du fichier...\n");
	wrefresh(subinfo);

	/* Hexa*/
	hexa = newwin(10, hexcols+2, 10, 0);
	box(hexa, ACS_VLINE, ACS_HLINE);
	wprintw(hexa, "Hex");
	wrefresh(hexa);

	subhexa = newwin(8, hexcols, 11, 1);
	wrefresh(subhexa);

	/* Characters*/
	character = newwin(10, charcols+2, 10, hexcols+2);
	box(character, ACS_VLINE, ACS_HLINE);
	wprintw(character, "Char");
	wrefresh(character);

	subchar = newwin(8, charcols, 11, hexcols+3);
	wrefresh(subchar);

	sleep(1);

	afficherFichier(fd, depart, nboctets, subhexa, subchar);
	move(curseur.y, curseur.x);
	wprintw(subinfo, "Chargement terminé.");
	wrefresh(subinfo);
	refresh();

	while((ch = getch()) != KEY_F(2)) {
		if(ch == KEY_DOWN) {
			if(depart+64 < nboctets) {
				depart += 8;
				afficherFichier(fd, depart, nboctets, subhexa, subchar);
				move(curseur.y, curseur.x);
				refresh();
			}
		}
		else if(ch == KEY_UP) {
			if(depart > 0) {
				depart -= 8;
				afficherFichier(fd, depart, nboctets, subhexa, subchar);
				move(curseur.y, curseur.x);
				refresh();
			}
		}
		else if(ch == KEY_LEFT) {
			/* Fenetre hexa */
			if(curseur.x > 1 && curseur.x <= 23) {
				curseur.x--;
				if(curseur.x%3 == 0)
					curseur.x--;
				move(curseur.y, curseur.x);
				refresh();
			}
			else if(curseur.x == 1 && curseur.y > 11) {
				curseur.x = 23;
				curseur.y--;
				move(curseur.y, curseur.x);
				refresh();
			}
			/* Fenetre char */
			else if(curseur.x > 27 && curseur.x <= 41) {
				curseur.x--;
				if(curseur.x%2 == 0)
					curseur.x--;
				move(curseur.y, curseur.x);
				refresh();
			}
			else if(curseur.x == 27 && curseur.y > 11) {
				curseur.x = 41;
				curseur.y--;
				move(curseur.y, curseur.x);
				refresh();
			}
		}
		else if(ch == KEY_RIGHT) {
			/* Fenetre hexa */
			if(curseur.x >= 1 && curseur.x < 23) {
				curseur.x++;
				if(curseur.x%3 == 0)
					curseur.x++;
				move(curseur.y, curseur.x);
				refresh();
			}
			else if(curseur.x == 23 && curseur.y < 18) {
				curseur.x = 1;
				curseur.y++;
				move(curseur.y, curseur.x);
				refresh();
			}
			/* Fenetre char */
			else if(curseur.x >= 27 && curseur.x < 41) {
				curseur.x++;
				if(curseur.x%2 == 0)
					curseur.x++;
				move(curseur.y, curseur.x);
				refresh();
			}
			else if(curseur.x == 41 && curseur.y < 18) {
				curseur.x = 27;
				curseur.y++;
				move(curseur.y, curseur.x);
				refresh();
			}
		}
		else if(ch == KEY_MOUSE) {
			souris_getpos(&curseur);
			/* Fenetre hexa */
			if(curseur.x >= 1 && curseur.x <= 23 && curseur.y >= 11 && curseur.y <= 18) {
				if(curseur.x % 3 != 0) {
					move(curseur.y, curseur.x);
					refresh();
				}
			}
			/* Fenetre char */
			else if(curseur.x >= 27 && curseur.x <= 41 && curseur.y >= 11 && curseur.y <= 18) {
				if(curseur.x % 2 != 0) {
					move(curseur.y, curseur.x);
					refresh();
				}
			}
			else {
				curseur.x = 1;
				curseur.y = 11;
				move(curseur.y, curseur.x);
				refresh();
			}
		}
		else {
			/* Fenetre hexa */
			if(curseur.x >= 1 && curseur.x <= 23 && curseur.y >= 11 && curseur.y <= 18) {
				if((ch >= 48 && ch <= 57) || (ch >= 97 && ch <= 102)) {
					lseek(fd, depart+((curseur.y-11)*8)+((curseur.x-1)/3), SEEK_SET);
					if(read(fd, &tmp, 1) == -1) {
						perror("Erreur lors de la lecture d'un caractère ");
						exit(EXIT_FAILURE);
					}
					/*Si c'est le premier caractère hexa d'un paquet*/
					if((curseur.x-1) % 3 == 0)
						tmp = asciiToHex(ch)*16 + tmp%16;
					else if((curseur.x-2) % 3 == 0)
						tmp = (tmp/16)*16 + asciiToHex(ch);
				}
			}
			/* Fenetre char */
			else if(curseur.x >= 27 && curseur.x <= 41 && curseur.y >= 11 && curseur.y <= 18) {
				if(ch >= 32 && ch <= 126) {
					lseek(fd, depart+((curseur.y-11)*8)+((curseur.x-27)/2), SEEK_SET);
					if(read(fd, &tmp, 1) == -1) {
						perror("Error ");
						exit(EXIT_FAILURE);
					}
					tmp = ch;
				}
			}

			/* écriture dans le fichier */
			lseek(fd, -1, SEEK_CUR);
			if(write(fd, &tmp, 1) == -1) {
				perror("Error ");
				exit(EXIT_FAILURE);
			}
			afficherFichier(fd, depart, nboctets, subhexa, subchar);
			move(curseur.y, curseur.x);
			refresh();
		}
	}

	/* Arrêt de ncurses */
	ncurses_stopper();
	if(close(fd) == -1) {
		perror("Erreur lors de la fermeture du fichier ");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
