#define _POSIX_SOURCE

#include <stdlib.h>     /* Pour exit, EXIT_SUCCESS, EXIT_FAILURE */
#include <sys/socket.h> /* Pour socket */
#include <arpa/inet.h>  /* Pour IPPROTO_TCP */
#include <stdio.h>      /* Pour perror */
#include <unistd.h>     /* Pour close */
#include <string.h>     /* Pour memset */
#include <time.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>

#include "requetes.h"
#include "serveur.h"

int stop = 0;

void handler(int sig) {
	if(sig == SIGCHLD) {
		pid_t p;
	    int status;

	    while ((p = waitpid(-1, &status, WNOHANG)) != -1)
	    {
	       kill(p, SIGINT);
	    }
	}
	if(sig == SIGINT) {
		stop = 1;
	}
}

int choisirCaseVide() {
	int i = 0;
	while (grille[i] != VIDE) { i++; }
	return i;
}

frontieres_t calculerFrontieres(int id) {
	frontieres_t etatFrontieres;

	if (id - LARGEUR <= 0) { etatFrontieres.haut = INEXISTANT; }
	else { etatFrontieres.haut = grille[id - LARGEUR]; }

	if (id % LARGEUR == 0) { etatFrontieres.gauche = INEXISTANT; }
	else { etatFrontieres.gauche = grille[id - 1]; }

	if (id + LARGEUR >= HAUTEUR*LARGEUR) { etatFrontieres.bas = INEXISTANT; }
	else { etatFrontieres.bas = grille[id + LARGEUR]; }

	if (id % LARGEUR == LARGEUR - 1) { etatFrontieres.droite = INEXISTANT; }
	else { etatFrontieres.droite = grille[id + 1]; }

	return etatFrontieres;
}

int main(int argc, char const *argv[]) {
	int fd, sockclient;
	struct sockaddr_in adresse;
	struct sigaction sig;

	sig.sa_handler = handler;
	sigaction(SIGCHLD, &sig, NULL);
	sigaction(SIGINT, &sig, NULL);

	/* Creation de la socket TCP */
	if((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		perror("Erreur lors de la creation de la socket ");
		exit(EXIT_FAILURE);
	}

	/* Creation de l'adresse du serveur */
	memset(&adresse, 0, sizeof(struct sockaddr_in));
	adresse.sin_family = AF_INET;
	adresse.sin_addr.s_addr = htonl(INADDR_ANY);
	adresse.sin_port = htons(PORT_CONNEXION);

	/* Nommage de la socket */
	if(bind(fd, (struct sockaddr*)&adresse, sizeof(struct sockaddr_in)) == -1) {
		perror("Erreur lors du nommage de la socket ");
		exit(EXIT_FAILURE);
	}

	/* Mise en mode passif de la socket */
	if(listen(fd, NB_MAX_CONNEXION) == -1) {
		perror("Erreur lors de la mise en mode passif ");
		exit(EXIT_FAILURE);
	}

	/* Attente d'une connexion */
	while(!stop) {
		_LOG("Attente de connexion")

		if((sockclient = accept(fd, NULL, NULL)) == -1) {
			perror("Erreur lors de la demande de connexion ");
			exit(EXIT_FAILURE);
		}

		_LOG("Nouvelle connexion reçue, création d'un nouveau fils")

		if(fork() == 0) {
			/* Code du fils, 1 par connexion */
			short type_requete;
			reponse_connexion_t reponseConnexion;

			_LOG("Nouvelle connexion, détermination du type de la requête")

			if(read(fd, &type_requete, sizeof(short)) == -1) {
				perror("Erreur lors de la lecture du type de la requête ");
				exit(EXIT_FAILURE);
			}

			switch (type_requete) {
				case TYPE_CONNEXION:
					/* Le client demande la création d'une zone */
					reponseConnexion.id = choisirCaseVide();
					reponseConnexion.etatFrontieres = calculerFrontieres(reponseConnexion.id);

					if (write(fd, &reponseConnexion, sizeof(reponse_connexion_t)) == -1) {
						perror("Erreur lors de l'envoi de la réponse de connexion");
					}

					break;
				case TYPE_FOURMI:
					/* Le client demande le passage d'une fourmi */
					break;
				default:
					_LOG("Erreur : Type de requête inconnu")
					exit(EXIT_FAILURE);
			}

			if(close(fd) == -1) {
				perror("Erreur lors de la fermeture de la socket de connexion ");
				exit(EXIT_FAILURE);
			}

			if(close(sockclient) == -1) {
				perror("Erreur lors de la fermeture de la socket de connexion ");
				exit(EXIT_FAILURE);
			}

			_LOG("Fin de la connexion, destruction du processus")

			exit(EXIT_SUCCESS);
		}
	}

	/* Fermeture des sockets */
	if(close(sockclient) == -1) {
		perror("Erreur lors de la fermeture de la socket de communication ");
		exit(EXIT_FAILURE);
	}
	if(close(fd) == -1) {
		perror("Erreur lors de la fermeture de la socket de connexion ");
		exit(EXIT_FAILURE);
	}

	printf("Serveur termine.\n");

	return EXIT_SUCCESS;
}
