/**
 * CS2106 AY 20/21 Semester 1 - Lab 3
 *
 * This file contains declarations. You should only modify the fifo_sem_t struct,
 * as the method signatures will be needed to compile with the runner.
 */
#include "exit_controller.h"
#include <stdlib.h>
#include <stdio.h>


void exit_controller_init(exit_controller_t *exit_controller, int no_of_priorities) {
    sem_init(&(exit_controller->exitSem), 1, 1);
    sem_init(&(exit_controller->queue), 1, 1);
    exit_controller->first = 0;
    exit_controller->last = 0;
    exit_controller->atom = 1;
    exit_controller->atom2 = 1;

    // for(int i = 0; i < MAX_PRIORITIES; i++) {
    //     sem_init(&exit_controller->arr[i], 1, 0);
    // }
}

void exit_controller_wait(exit_controller_t *exit_controller, int priority) {
    printf("Exit Wait Start\n");
    sem_wait(&exit_controller->queue); // Queue CS
    sem_t *node = malloc(sizeof(sem_t));
    if (exit_controller->atom > 0) {
        sem_init(node, 1, 1);
        exit_controller->atom--;
    } else {
        sem_init(node, 1, 0);
        enqueueX(exit_controller, node, priority);
    } 
    sem_post(&exit_controller->queue); // Queue CS
    sem_wait(node);
    sem_wait(&exit_controller->exitSem);
    printf("Exit Wait End\n");
}

void exit_controller_post(exit_controller_t *exit_controller, int priority) {
    printf("Exit Post Start\n");
    sem_wait(&exit_controller->queue); // Queue CS
    dequeueX(exit_controller);
    sem_post(&exit_controller->queue); // Queue CS
    sem_post(&exit_controller->exitSem);
    printf("Exit Post End\n");

}

void exit_controller_destroy(exit_controller_t *exit_controller){
    // for (int i = 0; i < MAX_PRIORITIES; i++) {
    //     sem_t *currSem = exit_controller->arr[i];
    //     // sem_destroy();
    //     // free(&currSem);
    // }
    // free(exit_controller);
}

void enqueueX(exit_controller_t *exit_controller, sem_t *node, int priority) {
    if (exit_controller->first - exit_controller->last == 0) {
        exit_controller->arr[exit_controller->first] = *node;
        exit_controller->last = (exit_controller->last + 1) % MAX_PRIORITIES;
    } else 
    if (priority == 0) {
        exit_controller->arr[(exit_controller->first - 1) % MAX_PRIORITIES] = *node;
        exit_controller->first = (exit_controller->first - 1) % MAX_PRIORITIES;
    } else {    
        exit_controller->arr[(exit_controller->last + 1) % MAX_PRIORITIES] = *node;
        exit_controller->last = (exit_controller->last + 1) % MAX_PRIORITIES;
    }
    
}

void dequeueX(exit_controller_t *exit_controller) {
    sem_t *node;
    if (exit_controller->first - exit_controller->last == 0) {
    } else {
        node = &exit_controller->arr[exit_controller->first];
        exit_controller->first = (exit_controller->first + 1) % MAX_PRIORITIES;
        sem_post(node);
    }
    
}