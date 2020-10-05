/**
 * CS2106 AY 20/21 Semester 1 - Lab 3
 *
 * Your implementation should go in this file.
 */
#include "fizzbuzz_workers.h"
#include "barrier.h" // you may use barriers if you think it can help your
                     // implementation

// declare variables to be used here
sem_t num_sem;
sem_t fizz_sem;
sem_t buzz_sem;
sem_t fizzbuzz_sem;
int count;
int curr;

void fizzbuzz_init ( int n ) {
    sem_init(&num_sem, 0, 1);
    sem_init(&fizz_sem, 0, 0);
    sem_init(&buzz_sem, 0, 0);
    sem_init(&fizzbuzz_sem, 0, 0);
    count = n;
    curr = 1;

}

void num_thread( int n, void (*print_num)(int) ) {
    
    while(curr != count) {
        sem_wait(&num_sem);
        print_num(curr);
        curr++;
        if (curr % 3 == 0) { // multiple of 3
            sem_post(&fizz_sem);
        } else if (curr % 5 == 0) { // multiple of 5
            sem_post(&buzz_sem);
        } else if (curr % 15 == 0) {
            sem_post(&fizzbuzz_sem); // multple of 3 and 5
        } else {
            sem_post(&num_sem);
        }
    }
    if (count == 1) {
        print_num(curr);
    }
}

void fizz_thread( int n, void (*print_fizz)(void) ) {
    while(curr != count) {
        sem_wait(&fizz_sem);
        print_fizz();
        curr++;
        if (curr % 3 == 0) { // multiple of 3
            sem_post(&fizz_sem);
        } else if (curr % 5 == 0) { // multiple of 5
            sem_post(&buzz_sem);
        } else if (curr % 15 == 0) {
            sem_post(&fizzbuzz_sem); // multple of 3 and 5
        } else {
            sem_post(&num_sem);
        }
    }
}

void buzz_thread( int n, void (*print_buzz)(void) ) {
    while(curr != count) {
        sem_wait(&buzz_sem);
        print_buzz();
        curr++;
        if (curr % 3 == 0) { // multiple of 3
            sem_post(&fizz_sem);
        } else if (curr % 5 == 0) { // multiple of 5
            sem_post(&buzz_sem);
        } else if (curr % 15 == 0) {
            sem_post(&fizzbuzz_sem); // multple of 3 and 5
        } else {
            sem_post(&num_sem);
        }
    }
}

void fizzbuzz_thread( int n, void (*print_fizzbuzz)(void) ) {
    while(curr != count) {
        sem_wait(&fizzbuzz_sem);
        print_fizzbuzz();
        curr++;
        if (curr % 3 == 0) { // multiple of 3
            sem_post(&fizz_sem);
        } else if (curr % 5 == 0) { // multiple of 5
            sem_post(&buzz_sem);
        } else if (curr % 15 == 0) {
            sem_post(&fizzbuzz_sem); // multple of 3 and 5
        } else {
            sem_post(&num_sem);
        }
    }
}

void fizzbuzz_destroy() {
    sem_destroy(&num_sem);
    sem_destroy(&fizz_sem);
    sem_destroy(&buzz_sem);
    sem_destroy(&fizzbuzz_sem);
    
}
