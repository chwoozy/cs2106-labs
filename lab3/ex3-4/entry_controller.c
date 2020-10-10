/**
 * CS2106 AY 20/21 Semester 1 - Lab 3
 *
 * Your implementation should go in this file.
 */
#include "entry_controller.h"
#include <stdlib.h>
#include <stdio.h>

sem_t queue, bay;
int atom;

void entry_controller_init( entry_controller_t *entry_controller, int loading_bays ) {
    // initialise queue
    sem_init(&queue, 1, 1);
    sem_init(&bay, 1, loading_bays);
    // entry_controller = malloc( sizeof(entry_controller_t));
    entry_controller->first = 0;
    entry_controller->last = 0;
    entry_controller->count = 0;
    entry_controller->queue = queue;
    entry_controller->atom = loading_bays;

}

void entry_controller_wait( entry_controller_t *entry_controller ) {
    sem_wait(&queue); // Queue CS
    sem_t node;
    node_t *currNode = malloc(sizeof(node_t));
    currNode->nodeSem = node;
    if (entry_controller->atom > 0) {
        sem_init(&node, 1, 1);
        entry_controller->atom--;
    } else {
        sem_init(&node, 1, 0);
        enqueue(entry_controller, currNode);
    }
    sem_post(&queue); // End Queue CS

    sem_wait(&node);
    sem_wait(&bay); // Bay CS
    sem_post(&bay);
    //Done
}

void entry_controller_post( entry_controller_t *entry_controller ) {
    sem_post(&bay);
    sem_wait(&queue); // Queue CS
    sem_t currSem = dequeue(entry_controller)->nodeSem;
    sem_post(&currSem);
    sem_post(&queue); // End Queue CS
}

void entry_controller_destroy( entry_controller_t *entry_controller ) {
    for (int i = 0; i < ENTRY_CONTROLLER_MAX_USES; i++) {
        node_t currSem = entry_controller->arr[i];
        sem_destroy(&(currSem.nodeSem));
    }
}

void enqueue(entry_controller_t *entry_controller, node_t *node) {
    entry_controller->arr[entry_controller->last] = *node;
    entry_controller->last = (entry_controller->last + 1) % ENTRY_CONTROLLER_MAX_USES;
    entry_controller->count++;
}

node_t* dequeue(entry_controller_t *entry_controller) {
    node_t *currNode = &(entry_controller->arr[entry_controller->first]);
    entry_controller->first = (entry_controller->first + 1) % ENTRY_CONTROLLER_MAX_USES;
    entry_controller->count--;
    return currNode;
}
