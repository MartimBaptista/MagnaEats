#include "synchronization.h"

#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>

sem_t * semaphore_create(char* name, int value){
    sem_t *semaphore;
    if ((semaphore = sem_open(name, O_CREAT, S_IROTH | S_IWOTH, value)) == SEM_FAILED) {
        perror ("sem_open"); 
        exit (1);
    }
    return semaphore;
}


void semaphore_destroy(char* name, sem_t* semaphore){
    if (sem_close (semaphore) == -1) {
        perror ("sem_close"); exit (1);
    }
    
    if (sem_unlink (name) == -1) {
        perror ("sem_unlink"); exit (1);
    }
}


void produce_begin(struct prodcons* pc){
    sem_wait(pc->mutex);
    sem_wait(pc->empty);
    sem_wait(pc->full); 
}


void produce_end(struct prodcons* pc){
    sem_post(pc->mutex);
    sem_post(pc->empty);
    sem_post(pc->full); 
}

void consume_begin(struct prodcons* pc){
    sem_wait(pc->mutex);
    sem_wait(pc->empty);
    sem_wait(pc->full); 
}


void consume_end(struct prodcons* pc){
    sem_post(pc->mutex);
    sem_post(pc->empty);
    sem_post(pc->full); 
}


void semaphore_mutex_lock(sem_t* sem){
    sem = Lock();
}


void semaphore_mutex_unlock(sem_t* sem){
    sem = Unlock();
}