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
    exit_controller->firstEnd = 0;
    exit_controller->last = (MAX_PRIORITIES * 2) - 1;
    exit_controller->lastEnd = (MAX_PRIORITIES * 2) - 1;
    exit_controller->atom = 1;

    for(int i = 0; i < MAX_PRIORITIES; i++) {
        sem_init(&exit_controller->arr[i], 1, 0);
    }
}

void exit_controller_wait(exit_controller_t *exit_controller, int priority) {
    printf("Exit Wait Start\n");
    sem_wait(&exit_controller->queue); // Queue CS
    sem_t* node = enqueueX(exit_controller, priority);
    if (exit_controller->atom > 0) {
        sem_init(node, 1, 1);
        dequeueX(exit_controller);
        exit_controller->atom--;
    } else {
        sem_init(node, 1, 0);
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

sem_t* enqueueX(exit_controller_t *exit_controller, int priority) {
    sem_t* node;
    if (priority == 0) {
        node = &exit_controller->arr[exit_controller->firstEnd];
        exit_controller->firstEnd++;
    } else {
        node = &exit_controller->arr[exit_controller->lastEnd];
        exit_controller->lastEnd--;
    }
    return node;
    
    // if (exit_controller->first - exit_controller->last == 0) {
    //     sem_t* node = &exit_controller->arr[exit_controller->first];
    //     exit_controller->last = (exit_controller->last + 1) % MAX_PRIORITIES;
    //     return node;
    // } else 
    // if (priority == 0) {
    //     sem_t* node = &exit_controller->arr[(exit_controller->first - 1) % MAX_PRIORITIES];
    //     exit_controller->first = (exit_controller->first - 1) % MAX_PRIORITIES;
    //     return node;
    // } else {    
    //     sem_t* node = &exit_controller->arr[(exit_controller->last + 1) % MAX_PRIORITIES];
    //     exit_controller->last = (exit_controller->last + 1) % MAX_PRIORITIES;
    //     return node;
    // }
    
}

void dequeueX(exit_controller_t *exit_controller) {
    sem_t *node;
    if(exit_controller->firstEnd - exit_controller->first != 0) {
        node = &exit_controller->arr[exit_controller->first];
        exit_controller->first++;
    } else {
        node = &exit_controller->arr[exit_controller->last];
        exit_controller->last--;
    }
    sem_post(node);
    // if (exit_controller->first - exit_controller->last == 0) {
    // } else {
    //     node = &exit_controller->arr[exit_controller->first];
    //     exit_controller->first = (exit_controller->first + 1) % MAX_PRIORITIES;
    //     sem_post(node); 
    // }  
}