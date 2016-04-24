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
	mvprintw(LINES-2, 23, "B");
	mvprintw(LINES-2, 41, "N");
	mvprintw(LINES-1, 0, "F2");
	mvprintw(LINES-1, 11, "F8");
	mvprintw(LINES-1, 26, "F10");
	attroff(COLOR_PAIR(4));

	mvprintw(LINES-2, 2, "Config. types fourmi");
	mvprintw(LINES-2, 25, "Config. bibites");
	mvprintw(LINES-2, 43, "Config. nourriture");
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

void dessinerFenetreZone(fichier_zone_t* zone) {
	int i;

	werase(editeur);

	for (i = 0; i < HAUTEUR*LARGEUR; i++) {
		switch (zone->grille[i]) {
			case VIDE :
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
}
void dessinerFenetreInfos();
void dessinerFenetreOutils() {
	mvwaddch(outil, 2, 4, ICONE_VIDE);
	mvwaddch(outil, 3, 4, ICONE_FOURMILIERE);
	mvwaddch(outil, 4, 4, ICONE_OBSTACLE);
	mvwprintw(outil, 0, 4, "Outil\tHotkey");
	mvwprintw(outil, 2, 6, "Vide\tV");
	mvwprintw(outil, 3, 6, "Fourmil.\tF");
	mvwprintw(outil, 4, 6, "Obstacle\tO");
}
void dessinerFenetreConfig(fichier_zone_t* zone) {
	int i;

	werase(config);
	wrefresh(config);

	mvwprintw(config, 1, 3, "Paramètre");
	mvwprintw(config, 3, 1, "Types de fourmi");
	mvwprintw(config, 4, 1, "Nb. de bibites");
	mvwprintw(config, 5, 1, "Tps. nourriture");
	mvwprintw(config, 6, 1, "Nb. de sources");
	mvwprintw(config, 7, 1, "Qté. nourriture");

	mvwprintw(config, 1, 22, "Valeur");
	mvwprintw(config, 3, 23, "%hhu", zone->nbTypesFourmi);
	mvwprintw(config, 4, 23, "%hhu", zone->nbBibites);
	mvwprintw(config, 5, 23, "%hu", zone->gestionnaireNourriture.delay);
	mvwprintw(config, 6, 23, "%hhu", zone->gestionnaireNourriture.nbSources);
	mvwprintw(config, 7, 23, "%hu", zone->gestionnaireNourriture.qteNourritureParSource);

	for (i = 0; i < LARGEUR_CONFIG; i++) {
		mvwaddch(config, 2, i, ACS_HLINE);
	}
	for (i = 0; i < HAUTEUR_CONFIG; i++) {
		mvwaddch(config, i, 19, ACS_VLINE);
	}
}

void initialiserFenetres() {
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

	dessinerFenetreOutils();
}

int chargerFichier(char* nomFichier, fichier_zone_t* zone) {
	int fd, i;

	if ((fd = open(nomFichier, O_RDWR)) == -1) {
		if (errno == ENOENT) {
			wprintw(infos, "Erreur lors du chargement du fichier : %s\n", ERREUR);
			return EXIT_FAILURE;
		}
	}

	for (i = 0; i < HAUTEUR*LARGEUR; i++) {
		if(read(fd, &zone->grille[i], sizeof(unsigned char)) == -1) {
			wprintw(infos, "Erreur lors de la lecture dans le fichier : %s\n", ERREUR);
			return EXIT_FAILURE;
		}
		switch (zone->grille[i]) {
		case FOURMILIERE:
			mvwaddch(editeur, i/LARGEUR, i%LARGEUR, ICONE_FOURMILIERE);
			break;
		case OBSTACLE:
			mvwaddch(editeur, i/LARGEUR, i%LARGEUR, ICONE_OBSTACLE);
			break;
		}
	}

	if(read(fd, &zone->nbTypesFourmi, sizeof(unsigned char)) == -1) {
		wprintw(infos, "Erreur lors de la lecture dans le fichier : %s\n", ERREUR);
		return EXIT_FAILURE;
	}

	for (i = 0; i < zone->nbTypesFourmi; i++) {
		if(read(fd, &zone->typesFourmi[i], sizeof(type_fourmi_t)) == -1) {
			wprintw(infos, "Erreur lors de la lecture dans le fichier : %s\n", ERREUR);
			return EXIT_FAILURE;
		}
	}

	if(read(fd, &zone->nbBibites, sizeof(unsigned char)) == -1) {
		wprintw(infos, "Erreur lors de la lecture dans le fichier : %s\n", ERREUR);
		return EXIT_FAILURE;
	}

	for (i = 0; i < zone->nbBibites; i++) {
		if(read(fd, &zone->bibites[i], sizeof(bibite_t)) == -1) {
			wprintw(infos, "Erreur lors de la lecture dans le fichier : %s\n", ERREUR);
			return EXIT_FAILURE;
		}
	}

	if(read(fd, &zone->gestionnaireNourriture, sizeof(gestionnaire_nourriture_t)) == -1) {
		wprintw(infos, "Erreur lors de la lecture dans le fichier : %s\n", ERREUR);
		return EXIT_FAILURE;
	}

	if (close(fd) == -1) {
		wprintw(infos, "Erreur lors de la fermeture du fichier : %s\n", ERREUR);
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
	int i, ch = 0, choix = 0;
	type_fourmi_t type = {"", 0, 0, 0, 0, 0};
	werase(config);

	mvwprintw(config, 1, 1, "%d. Ajouter un type", AJOUTER);
	mvwprintw(config, 3, 1, "%d. Modifier un type", MODIFIER);
	mvwprintw(config, 5, 1, "Appuyez sur Esc. pour revenir");

	wrefresh(config);

	while((ch = getch()) != 27) {
		switch (ch) {
			case '1':
				werase(config);
				if(zone->nbTypesFourmi >= NB_TYPES_FOURMI_MAX) {
					wprintw(infos, "Le nombre maximum de types de fourmi a déjà été déclaré.\n");
					werase(config);
					dessinerFenetreConfig(zone);
					return;
				}
				writing_mode(true);
				mvwprintw(config, 1, 1, "Nom du type (15 cara. max) :");
				wrefresh(config);
				mvwscanw(config, 3, 1, "%15s", &type.nom);
				if(strlen(type.nom) == 0) strcpy(type.nom, "Default");
				werase(config);
				mvwprintw(config, 1, 1, "Id du comportement perso. :");
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hhu", &type.typePerso);
				werase(config);
				mvwprintw(config, 1, 1, "Vitesse :");
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hu", &type.vitesse);
				werase(config);
				mvwprintw(config, 1, 1, "PV maximum :");
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hu", &type.pvMax);
				werase(config);
				mvwprintw(config, 1, 1, "Attaque :");
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hu", &type.attaque);
				werase(config);
				mvwprintw(config, 1, 1, "Capacité de transport :");
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hu", &type.capacite);
				zone->typesFourmi[zone->nbTypesFourmi] = type;
				zone->nbTypesFourmi = zone->nbTypesFourmi + 1;
				wprintw(infos, "Type ajouté\n");
				werase(config);
				dessinerFenetreConfig(zone);
				writing_mode(false);
				refreshAll();
				return;
				break;
			case '2':
				if (zone->nbTypesFourmi == 0) {
					wprintw(infos, "Il n'y a aucun type de fourmi à modifier\n");
					werase(config);
					dessinerFenetreConfig(zone);
					return;
				}
				werase(config);
				for (i = 0; i < zone->nbTypesFourmi; i++) {
					mvwprintw(config, i, 1, "%d. %s", i, zone->typesFourmi[i].nom);
				}
				wrefresh(config);
				while((choix = getch()) < 48 || choix > 57-(10-zone->nbTypesFourmi)) {}
				choix -= 48;
				werase(config);
				writing_mode(true);
				mvwprintw(config, 1, 1, "Nom du type (15 cara. max) :");
				mvwprintw(config, 5, 1, "Actuel : %s", zone->typesFourmi[choix].nom);
				wrefresh(config);
				mvwscanw(config, 3, 1, "%15s", &type.nom);
				werase(config);
				mvwprintw(config, 1, 1, "Id du comportement perso. :");
				mvwprintw(config, 5, 1, "Actuel : %hhu", zone->typesFourmi[choix].typePerso);
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hhu", &type.typePerso);
				werase(config);
				mvwprintw(config, 1, 1, "Vitesse :");
				mvwprintw(config, 5, 1, "Actuel : %hu", zone->typesFourmi[choix].vitesse);
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hu", &type.vitesse);
				werase(config);
				mvwprintw(config, 1, 1, "PV maximum :");
				mvwprintw(config, 5, 1, "Actuel : %hu", zone->typesFourmi[choix].pvMax);
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hu", &type.pvMax);
				werase(config);
				mvwprintw(config, 1, 1, "Attaque :");
				mvwprintw(config, 5, 1, "Actuel : %hu", zone->typesFourmi[choix].attaque);
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hu", &type.attaque);
				werase(config);
				mvwprintw(config, 1, 1, "Capacité de transport :");
				mvwprintw(config, 5, 1, "Actuel : %hu", zone->typesFourmi[choix].capacite);
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hu", &type.capacite);
				zone->typesFourmi[choix] = type;
				wprintw(infos, "Type %d modifié\n", choix);
				werase(config);
				dessinerFenetreConfig(zone);
				writing_mode(false);
				refreshAll();
				return;
				break;
		}
	}

	werase(config);
	dessinerFenetreConfig(zone);
	refreshAll();
}

void configurerBibites(fichier_zone_t* zone) {
	int ch = 0;
	unsigned short choix = 0;
	bibite_t bibite = {0, 0, 0, 0};
	werase(config);

	mvwprintw(config, 1, 1, "1. Ajouter une bibite");
	mvwprintw(config, 3, 1, "2. Modifier une bibite");
	mvwprintw(config, 5, 1, "Appuyez sur Esc. pour revenir");

	wrefresh(config);

	while((ch = getch()) != 27) {
		switch (ch) {
			case '1':
				werase(config);
				if(zone->nbBibites >= NB_BIBITES_MAX) {
					wprintw(infos, "Le nombre maximum de types de bibites a déjà été déclaré.\n");
					werase(config);
					dessinerFenetreConfig(zone);
					return;
				}
				writing_mode(true);

				mvwprintw(config, 1, 1, "Id du comportement perso. :");
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hhu", &bibite.typePerso);
				werase(config);

				mvwprintw(config, 1, 1, "Vitesse :");
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hu", &bibite.vitesse);
				werase(config);

				mvwprintw(config, 1, 1, "PV maximum :");
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hu", &bibite.pvMax);
				werase(config);

				mvwprintw(config, 1, 1, "Attaque :");
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hu", &bibite.attaque);
				werase(config);

				zone->bibites[zone->nbBibites] = bibite;
				zone->nbBibites++;

				wprintw(infos, "Bibite %hu ajouté\n", zone->nbBibites-1);
				werase(config);
				dessinerFenetreConfig(zone);

				writing_mode(false);
				refreshAll();
				return;
				break;
			case '2':
				if (zone->nbBibites == 0) {
					wprintw(infos, "Il n'y a aucune bibite à modifier\n");
					werase(config);
					dessinerFenetreConfig(zone);
					return;
				}
				werase(config);
				mvwprintw(config, 1, 1, "Numéro de la bibite à modifier :");
				wrefresh(config);

				writing_mode(true);
				mvwscanw(config, 3, 1, "%hu", &choix);

				if (choix >= zone->nbBibites) {
					writing_mode(false);
					wprintw(infos, "La bibite %hu n'existe pas\n", choix);
					werase(config);
					dessinerFenetreConfig(zone);
					return;
				}

				werase(config);
				mvwprintw(config, 1, 1, "Id du comportement perso. :");
				mvwprintw(config, 5, 1, "Actuel : %hhu", zone->bibites[choix].typePerso);
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hhu", &zone->bibites[choix].typePerso);
				werase(config);

				mvwprintw(config, 1, 1, "Vitesse :");
				mvwprintw(config, 5, 1, "Actuel : %hu", zone->bibites[choix].vitesse);
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hu", &zone->bibites[choix].vitesse);
				werase(config);

				mvwprintw(config, 1, 1, "PV maximum :");
				mvwprintw(config, 5, 1, "Actuel : %hu", zone->bibites[choix].pvMax);
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hu", &zone->bibites[choix].pvMax);
				werase(config);

				mvwprintw(config, 1, 1, "Attaque :");
				mvwprintw(config, 5, 1, "Actuel : %hu", zone->bibites[choix].attaque);
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hu", &zone->bibites[choix].attaque);
				werase(config);

				wprintw(infos, "Type %hu modifié\n", choix);
				werase(config);
				dessinerFenetreConfig(zone);

				writing_mode(false);
				refreshAll();
				return;
				break;
		}
	}

	werase(config);
	dessinerFenetreConfig(zone);
	refreshAll();
}

void configurerGestionnaireNourriture(fichier_zone_t* zone) {
	int ch = 0;
	werase(config);

	wrefresh(config);

	mvwprintw(config, 1, 1, "1. Periodicité d'apparition");
	mvwprintw(config, 3, 1, "2. Nombre de sources");
	mvwprintw(config, 5, 1, "3. Qté. par source");
	mvwprintw(config, 7, 1, "Appuyez sur Esc. pour revenir");

	wrefresh(config);

	while((ch = getch()) != 27) {
		switch (ch) {
			case '1':
				werase(config);
				writing_mode(true);

				mvwprintw(config, 1, 1, "Periodicité d'apparition :");
				mvwprintw(config, 5, 1, "Actuel : %hus", zone->gestionnaireNourriture.delay);
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hu", &zone->gestionnaireNourriture.delay);
				werase(config);

				wprintw(infos, "Périodicité configurée : %hu secondes\n", zone->gestionnaireNourriture.delay);

				writing_mode(false);
				ch = 27;

				break;
			case '2':
				werase(config);
				writing_mode(true);

				mvwprintw(config, 1, 1, "Nombre de sources max. :");
				mvwprintw(config, 5, 1, "Actuel : %hhu", zone->gestionnaireNourriture.nbSources);
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hhu", &zone->gestionnaireNourriture.nbSources);
				werase(config);

				wprintw(infos, "Nombre de sources max. configuré : %hhu\n", zone->gestionnaireNourriture.nbSources);

				writing_mode(false);
				ch = 27;

				break;
			case '3':
				werase(config);
				writing_mode(true);

				mvwprintw(config, 1, 1, "Qté. nourriture par source :");
				mvwprintw(config, 5, 1, "Actuel : %hu", zone->gestionnaireNourriture.qteNourritureParSource);
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hu", &zone->gestionnaireNourriture.qteNourritureParSource);
				werase(config);

				wprintw(infos, "Qté. de nourriture par source configurée : %hu\n", zone->gestionnaireNourriture.qteNourritureParSource);

				writing_mode(false);
				ch = 27;

				break;
		}
	}

	dessinerFenetreConfig(zone);
	refreshAll();
}

void configurerFourmiliere(fichier_zone_t* zone) {
	int ch = 0;
	souris_t curseur;
	werase(config);

	wrefresh(config);

	mvwprintw(config, 1, 1, "Cliquez sur une foumilière pour la configurer");
	mvwprintw(config, 3, 1, "Appuyez sur Esc. pour revenir");

	wrefresh(config);

	while((ch = getch()) != 27) {
		switch (ch) {
			case KEY_MOUSE:
				souris_getpos(&curseur);
				if (isInZoneWindow(curseur)) {
					werase(config);
				}
				else {
					wprintw(infos, "La case n'est pas une fourmilière.");
					dessinerFenetreConfig(zone);
					return;
				}
				werase(config);
				writing_mode(true);

				mvwprintw(config, 1, 1, "Periodicité d'apparition :");
				mvwprintw(config, 5, 1, "Actuel : %hus", zone->gestionnaireNourriture.delay);
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hu", &zone->gestionnaireNourriture.delay);
				werase(config);

				wprintw(infos, "Périodicité configurée : %hu secondes\n", zone->gestionnaireNourriture.delay);
				werase(config);
				dessinerFenetreConfig(zone);

				writing_mode(false);
				refreshAll();

				return;
				break;
			case '2':
				werase(config);
				writing_mode(true);

				mvwprintw(config, 1, 1, "Nombre de sources max. :");
				mvwprintw(config, 5, 1, "Actuel : %hhu", zone->gestionnaireNourriture.nbSources);
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hhu", &zone->gestionnaireNourriture.nbSources);
				werase(config);

				wprintw(infos, "Nombre de sources max. configuré : %hhu\n", zone->gestionnaireNourriture.nbSources);
				werase(config);
				dessinerFenetreConfig(zone);

				writing_mode(false);
				refreshAll();

				return;
				break;
			case '3':
				werase(config);
				writing_mode(true);

				mvwprintw(config, 1, 1, "Qté. nourriture par source :");
				mvwprintw(config, 5, 1, "Actuel : %hu", zone->gestionnaireNourriture.qteNourritureParSource);
				wrefresh(config);
				mvwscanw(config, 3, 1, "%hu", &zone->gestionnaireNourriture.qteNourritureParSource);
				werase(config);

				wprintw(infos, "Qté. de nourriture par source configurée : %hu\n", zone->gestionnaireNourriture.qteNourritureParSource);
				werase(config);
				dessinerFenetreConfig(zone);

				writing_mode(false);
				refreshAll();

				return;
				break;
		}
	}

	werase(config);
	dessinerFenetreConfig(zone);
	refreshAll();
}

int ecrireFichier(int fd, fichier_zone_t* zone) {
	int i;

	for (i = 0; i < HAUTEUR*LARGEUR; i++) {
		if (write(fd, &zone->grille[i], sizeof(char)) == -1) {
			wprintw(infos, "Erreur lors de l'écriture dans le fichier : %s\n", ERREUR);
			return EXIT_FAILURE;
		}
	}
	if (write(fd, &zone->nbTypesFourmi, sizeof(unsigned char)) == -1) {
		wprintw(infos, "Erreur lors de l'écriture dans le fichier : %s\n", ERREUR);
		return EXIT_FAILURE;
	}
	for (i = 0; i < zone->nbTypesFourmi; i++) {
		if (write(fd, &zone->typesFourmi[i], sizeof(type_fourmi_t)) == -1) {
			wprintw(infos, "Erreur lors de l'écriture dans le fichier : %s\n", ERREUR);
			return EXIT_FAILURE;
		}
	}
	if (write(fd, &zone->nbBibites, sizeof(unsigned char)) == -1) {
		wprintw(infos, "Erreur lors de l'écriture dans le fichier : %s\n", ERREUR);
		return EXIT_FAILURE;
	}
	for (i = 0; i < zone->nbBibites; i++) {
		if (write(fd, &zone->bibites[i], sizeof(bibite_t)) == -1) {
			wprintw(infos, "Erreur lors de l'écriture dans le fichier : %s\n", ERREUR);
			return EXIT_FAILURE;
		}
	}
	if (write(fd, &zone->gestionnaireNourriture, sizeof(gestionnaire_nourriture_t)) == -1) {
		wprintw(infos, "Erreur lors de l'écriture dans le fichier : %s\n", ERREUR);
		return EXIT_FAILURE;
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
