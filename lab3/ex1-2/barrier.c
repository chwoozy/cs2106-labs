/**
 * CS2106 AY 20/21 Semester 1 - Lab 3
 *
 * This file contains function definitions. Your implementation should go in
 * this file.
 */

#include "barrier.h"

// Initialise barrier here  
void barrier_init ( barrier_t *barrier, int count ) {
    sem_t sem;
    sem_t mutex;
    sem_init(&sem, 1, 0);
    sem_init(&mutex, 1, 1);
    barrier->sem = sem;
    barrier->mutex = mutex;
    barrier->count = count;
}

void barrier_wait ( barrier_t *barrier ) {
    sem_wait(&(barrier->mutex));
    barrier->count--;
    //add one more sem for mutex since there may be a race condition
    if (barrier->count == 0) {
        sem_post(&(barrier->sem));
    }
    sem_post(&(barrier->mutex));
    sem_wait(&(barrier->sem));
    sem_post(&(barrier->sem));
    
}

// Perform cleanup here if you need to
void barrier_destroy ( barrier_t *barrier ) {
    sem_destroy(&(barrier->sem));
}
