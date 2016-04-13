#define _XOPEN_SOURCE
#include <stdio.h>      /* Pour printf, perror */
#include <stdlib.h>     /* Pour exit, EXIT_SUCCESS, EXIT_FAILURE */
#include <sys/shm.h>    /* Pour shmget, shmat, shmdt */
#include <errno.h>      /* Pour errno */
#include <sys/stat.h>   /* Pour S_IRUSR, S_IWUSR */
#include <sys/sem.h> 	/* Pour semget, semctl, semop */
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include "zone.h"
#include "../header/memoirePartagee.h"
#include "../header/semaphores.h"

key_t cle;
void gestionnaire(int signum){
	if(signum == SIGINT){
		/*detruireTerrain(cle);*/
		exit(EXIT_SUCCESS);
	}
}
int main(int argc, const char * argv[]) {
	int semid;
	struct sigaction sig;
	segment_t* terrain;

	if(argc != 2){
		fprintf(stderr, "Entrer la cle utilisée par le tableau de semaphores et le segment de memoire partage \n");
		exit(EXIT_FAILURE);
	}

	cle = (key_t)atoi(argv[1]);

	sig.sa_handler = gestionnaire;
	sigfillset(&sig.sa_mask);
	sig.sa_flags = 0;
	sigaction(SIGINT, &sig, 0);
	if((semid = semget(cle, 0, 0)) == -1) {
        perror("Erreur lors de la récupération du tableau de semaphores ");
    	exit(EXIT_FAILURE);
    }


	terrain = recupererTerrain(cle);



	while (1) {
		sleep(1);
		P(semid, 1, 1);
		if(terrain->cases[10] == 1){
			terrain->cases[10] = 0;
		}else{
			terrain->cases[10] = 1;
		}
		V(semid, 0,1);
	}

    exit(EXIT_SUCCESS);
}
