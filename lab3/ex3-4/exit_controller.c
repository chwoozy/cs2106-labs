/**
 * CS2106 AY 20/21 Semester 1 - Lab 3
 *
 * This file contains declarations. You should only modify the fifo_sem_t struct,
 * as the method signatures will be needed to compile with the runner.
 */
#include "exit_controller.h"
#include <stdlib.h>

sem_t exitSem, queue;
int atom;

void exit_controller_init(exit_controller_t *exit_controller, int no_of_priorities) {
    // initialise queue
    sem_init(&exitSem, 1, 1);
    sem_init(&exitSem, 1, 1);
    exit_controller = malloc( sizeof(exit_controller_t));
    exit_controller->first = 0;
    exit_controller->last = 0;
    exit_controller->queue = queue;
    exit_controller->exitSem = exitSem;

    // initialise atomic
    atom = 1;
}

void exit_controller_wait(exit_controller_t *exit_controller, int priority) {
    sem_wait(&queue); // Start Queue
    sem_t node;
    if (atom > 0) {
        sem_init(&node, 1, 1);
    } else {
        sem_init(&node, 1, 0);
    }
    nodeX_t *currNode = malloc(sizeof(nodeX_t));
    currNode->nodeSem = node;
    enqueueX(exit_controller, currNode, priority);
    sem_post(&queue); // End Queue

    sem_wait(&node);
}

void exit_controller_post(exit_controller_t *exit_controller, int priority) {
    sem_wait(&queue); // Queue CS
    // sem_post(&exit);
    sem_t currSem = dequeueX(exit_controller).nodeSem;
    sem_post(&currSem);
    sem_post(&queue); // End Queue CS
}

void exit_controller_destroy(exit_controller_t *exit_controller){
    for (int i = 0; i < MAX_PRIORITIES; i++) {
        nodeX_t currSem = exit_controller->arr[i];
        sem_destroy(&(currSem.nodeSem));
        free(&currSem);
    }
    free(exit_controller);
}

void enqueueX(exit_controller_t *exit_controller, nodeX_t *node, int priority) {
    if (priority == 0) {
        exit_controller->arr[exit_controller->first - 1] = *node;
        exit_controller->first = (exit_controller->first - 1) % MAX_PRIORITIES;
    } else {
        exit_controller->arr[exit_controller->last + 1] = *node;
        exit_controller->last = (exit_controller->last + 1) % MAX_PRIORITIES;
    }
    
}

nodeX_t dequeueX(exit_controller_t *exit_controller) {
    nodeX_t currNode = exit_controller->arr[exit_controller->first];
    exit_controller->first = (exit_controller->first + 1) % MAX_PRIORITIES;
    return currNode;
}
