#include "semaphores.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>  /* Pour semget, semctl, semop */
#include <errno.h>    /* Pour errno */
#include <sys/stat.h> /* Pour S_IRUSR, S_IWUSR */
#include <sys/msg.h>    /* Pour msgget, msgsnd, msgrcv */


void init(int semid, int sem_num, int val) {
	if(semctl(semid, sem_num, SETVAL, val) == -1) {
      perror("Erreur lors de l'initialisation des semaphores ");
      exit(EXIT_FAILURE);
    }
}

void P(int semid, int sem_num, int sem_op) {
	struct sembuf op;
	op.sem_num = sem_num;
    op.sem_op = 0 - sem_op;
    op.sem_flg = 0;
    if(semop(semid, &op, 1) == -1) {
      perror("Erreur lors de l'operation sur le semaphore ");
      exit(EXIT_FAILURE);
    }
}

void V(int semid, int sem_num, int sem_op) {
	struct sembuf op;
	op.sem_num = sem_num;
    op.sem_op = sem_op;
    op.sem_flg = 0;
    if(semop(semid, &op, 1) == -1) {
      perror("Erreur lors de l'operation sur le semaphore ");
      exit(EXIT_FAILURE);
    }
}

void destroy(int semid) {
	if(semctl(semid, 0, IPC_RMID) == -1) {
		perror("Erreur lors de la suppresion du tableau de semaphores ");
		exit(EXIT_FAILURE);
	}
}
