#ifndef __SEMAPHORES_H__
#define __SEMAPHORES_H__

#define _XOPEN_SOURCE


void init(int semid, int sem_num, int val);
void P(int semid, int sem_num, int sem_op) ;
void V(int semid, int sem_num, int sem_op);
void destroy(int semid);

#endif
