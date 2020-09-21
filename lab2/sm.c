/**
 * CS2106 AY 20/21 Semester 1 - Lab 2
 *
 * This file contains function definitions. Your implementation should go in
 * this file.
 */

#include "sm.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>

sm_status_t processArr[SM_MAX_SERVICES];
size_t currProcess;

// Use this function to any initialisation if you need to.
void sm_init(void) {
    currProcess = 0;
}

// Use this function to do any cleanup of resources.
void sm_free(void) {
}

// Exercise 1a/2: start services
void sm_start(const char *processes[]) {
    int processCount = 0;
    // count commands
    char** cmdCountPtr = (char**) &processes[0];
    int cmdCount = 0;
    while (*cmdCountPtr != NULL) {
        for (size_t i = 0; i < SM_MAX_SERVICES; i++) {
            if (*cmdCountPtr == NULL) {
                cmdCountPtr++;
                cmdCount++;
                break;
            } else {
                cmdCountPtr++;
            }
        }
    }
    
    // create and store pipes
    int fds[cmdCount - 1][2];
    for (int i = 0; i < cmdCount - 1; i++) {
        if (pipe(fds[i]) == -1) {
            printf("Pipe Creation Failed");
        }
    }

    char** currPtr = (char**) &processes[0];
    char *path;
    pid_t finalpid;
    // stops when the next one is a NULL
    while(*currPtr != NULL) {
        char *singleProcess[SM_MAX_SERVICES];
        path = *currPtr;

        for(size_t i = 0; i < SM_MAX_SERVICES; i++) {
            if (*currPtr == NULL) {
                singleProcess[i] = NULL;
                currPtr++;
                break;
            } else {
                singleProcess[i] = *currPtr;
                currPtr++;
            }
        }
        pid_t pid = fork();
        
        if (pid == 0) {
            if (processCount == 0) {
                close(fds[processCount][0]);
                dup2(fds[processCount][1], STDOUT_FILENO);
            } else if (processCount == cmdCount - 1) {
                close(fds[processCount - 1][1]);
                dup2(fds[processCount - 1][0], STDIN_FILENO);
            } else {
                close(fds[processCount - 1][1]);
                dup2(fds[processCount - 1][0], STDIN_FILENO);
                close(fds[processCount][0]);
                dup2(fds[processCount][1], STDOUT_FILENO);
            }
            execv(singleProcess[0], (char *const *) singleProcess);
        }

        // close all pipes
        if (processCount != cmdCount - 1) {
            close(fds[processCount][0]);
            close(fds[processCount][1]);
        } else {
            finalpid = pid;
        }
        

        
        processCount++;
    }

    // initialise current status
    sm_status_t currStatus;
    currStatus.path = path;
    currStatus.pid = finalpid;
    currStatus.running = true;

    // save state
    processArr[currProcess] = currStatus;
    
    // update counter
    currProcess++;
}

// Exercise 1b: print service status
size_t sm_status(sm_status_t statuses[]) {
    int status;
    for(size_t i = 0; i < currProcess; i++) {
        if (waitpid(processArr[i].pid, &status, WNOHANG) != 0) {
            processArr[i].running = false;
        } else {
            processArr[i].running = true;
        }
        statuses[i] = processArr[i];
    }
    return currProcess;
}

// Exercise 3: stop service, wait on service, and shutdown
void sm_stop(size_t index) {
}

void sm_wait(size_t index) {
}

void sm_shutdown(void) {
}

// Exercise 4: start with output redirection
void sm_startlog(const char *processes[]) {
}

// Exercise 5: show log file
void sm_showlog(size_t index) {
}
