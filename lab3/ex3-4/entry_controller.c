/**
 * CS2106 AY 20/21 Semester 1 - Lab 3
 *
 * Your implementation should go in this file.
 */
#include "entry_controller.h"
#include <stdlib.h>
#include <stdio.h>

void entry_controller_init( entry_controller_t *entry_controller, int loading_bays ) {
    sem_init(&entry_controller->queue, 1, 1);
    sem_init(&entry_controller->bay, 1, loading_bays);
    entry_controller->first = 0;
    entry_controller->last = loading_bays;
    entry_controller->begin = loading_bays;

    int atom = loading_bays;
    for (int i = 0; i < ENTRY_CONTROLLER_MAX_USES; i++) {
        if (atom > 0) {
            sem_init(&entry_controller->arr[i], 1, 1);
            atom--;
        } else {
            sem_init(&entry_controller->arr[i], 1, 0);
        }
    }
}

void entry_controller_wait( entry_controller_t *entry_controller ) {
    sem_wait(&entry_controller->queue); // Queue CS
    sem_t *node = enqueue(entry_controller);
    sem_post(&entry_controller->queue); // End Queue CS

    sem_wait(node);
    sem_wait(&entry_controller->bay);
}

void entry_controller_post( entry_controller_t *entry_controller ) {
    sem_wait(&entry_controller->queue); // Queue CS
    sem_t *node = dequeue(entry_controller);
    sem_post(node);
    sem_post(&entry_controller->queue); // End Queue CS
    sem_post(&entry_controller->bay);
}

void entry_controller_destroy( entry_controller_t *entry_controller ) {
    for (int i = 0; i < ENTRY_CONTROLLER_MAX_USES; i++) {
        sem_t currSem = entry_controller->arr[i];
        sem_destroy(&currSem);
    }
    free(entry_controller);
}

sem_t* enqueue(entry_controller_t *entry_controller) {
    sem_t* node = &entry_controller->arr[entry_controller->first];
    entry_controller->first = (entry_controller->first + 1) % ENTRY_CONTROLLER_MAX_USES;
    return node;
}

sem_t* dequeue(entry_controller_t *entry_controller) {
    sem_t *node = &entry_controller->arr[entry_controller->last];
    entry_controller->last = (entry_controller->last + 1) % ENTRY_CONTROLLER_MAX_USES;
    return node;
}
