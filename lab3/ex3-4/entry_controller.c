/**
 * CS2106 AY 20/21 Semester 1 - Lab 3
 *
 * Your implementation should go in this file.
 */
#include "entry_controller.h"
#include <stdlib.h>
#include <stdio.h>

sem_t queue, bay;

void entry_controller_init( entry_controller_t *entry_controller, int loading_bays ) {
    sem_init(&queue, 1, 1);
    sem_init(&bay, 1, loading_bays);
    entry_controller->first = 0;
    entry_controller->last = 0;
    entry_controller->queue = queue;
    entry_controller->atom = loading_bays;

}

void entry_controller_wait( entry_controller_t *entry_controller ) {
    sem_wait(&queue); // Queue CS
    sem_t node = malloc(sizeof(sem_t));
    if (entry_controller->atom > 0) {
        sem_init(&node, 1, 1);
        entry_controller->atom--;
    } else {
        sem_init(&node, 1, 0);
        enqueue(entry_controller, &node);
    }
    sem_post(&queue); // End Queue CS
    sem_wait(&node);
    sem_wait(&bay); // Bay CS
}

void entry_controller_post( entry_controller_t *entry_controller ) {
    sem_wait(&queue); // Queue CS
    sem_t *currSem = dequeue(entry_controller);
    sem_post(currSem);
    sem_post(&queue); // End Queue CS
    sem_post(&bay);
}

void entry_controller_destroy( entry_controller_t *entry_controller ) {
    // printf("Entry Destroy");
    // // for (int i = 0; i < ENTRY_CONTROLLER_MAX_USES; i++) {
    // //     sem_t *currSem = entry_controller->arr[i];
    // //     sem_destroy(currSem);
    // // }
    // free(entry_controller);
}

void enqueue(entry_controller_t *entry_controller, sem_t *node) {
    entry_controller->arr[entry_controller->last] = node;
    entry_controller->last = (entry_controller->last + 1) % ENTRY_CONTROLLER_MAX_USES;
}

sem_t* dequeue(entry_controller_t *entry_controller) {
    sem_t *currNode = entry_controller->arr[entry_controller->first];
    entry_controller->first = (entry_controller->first + 1) % ENTRY_CONTROLLER_MAX_USES;
    return currNode;
}
