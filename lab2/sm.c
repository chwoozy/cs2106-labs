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

#define ERROR -1
#define READ 0
#define WRITE 1
#define CHILD 0
#define PARENT != 0
#define start 0
#define middle 1
#define end 2
#define single 3

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
    char *path;
    while (*cmdCountPtr != NULL) {
        path = *cmdCountPtr;
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
    if (cmdCount > 1) { 
        for (int i = 0; i < cmdCount - 1; i++) {
            if (pipe(fds[i]) == -1) {
                perror("[CW] Pipe Creation Failed\n");
                break;
            }
        }
    }
    

    char** currPtr = (char**) &processes[0];
    pid_t finalpid;

    // process commands
    while(*currPtr != NULL) {
        // retrieve single command
        char *singleProcess[SM_MAX_SERVICES];
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
        int scenario = cmdCount == 1 ? 3 : processCount == 0 ? 0 : processCount == (cmdCount - 1) ? 2 : 1;
        // begin command processing
        pid_t pid;
        switch (pid = fork()) {
            case ERROR: 
                perror("[CW] Error in creating child processes\n"); 
                break;

            case CHILD:

                for(int i = 0; i < cmdCount - 1; i++) {
                    if (i != processCount) {
                        switch (scenario) {
                            case single:
                                break;
                            case start:
                                close(fds[i][READ]);
                                close(fds[i][WRITE]);
                                break;
                            case middle:
                                if (i != (processCount - 1)) {
                                    close(fds[i][READ]);
                                    close(fds[i][WRITE]);
                                }
                                break;
                            case end:
                                if (i != (processCount - 1)) {
                                    close(fds[i][READ]);
                                    close(fds[i][WRITE]);
                                }
                                break;
                        }
                    }
                }
                
                switch (scenario) {
                    case single: 
                        break;

                    case start:
                        close(fds[processCount][READ]);
                        // int sfd = dup(STDOUT_FILENO);
                        // if (sfd == ERROR) {
                        //     perror("[CW] Start dup error: fetching output\n");
                        // } else {
                        //     if (dup2(sfd, fds[processCount][WRITE]) == ERROR) {
                        //         perror("[CW] Start dup error: write into pipe\n");
                        //     };
                        // }
                        close(STDOUT_FILENO);
                        if (dup(fds[processCount][WRITE]) == ERROR) {
                            perror("[CW] Start dup error: write into pipe\n");
                        };
                        // close(fds[processCount][WRITE]);
                        break;

                    case middle:
                        close(fds[processCount - 1][WRITE]);
                        close(fds[processCount][READ]);
                        
                        // if (dup2(fds[processCount - 1][READ], STDIN_FILENO) == ERROR) {
                            
                        //     perror("[CW] Middle dup error: replacing input\n");
                        // }
                        // int mfd = dup(STDOUT_FILENO);
                        // if (mfd == ERROR) {
                        //     perror("[CW] Middle dup error: fetching output\n");
                        // } else {
                        //     if (dup2(mfd, fds[processCount][WRITE]) == ERROR) {
                        //         perror("[CW] Middle dup error: write into pipe\n");
                        //     }
                        // }
                        close(STDIN_FILENO);
                        if (dup(fds[processCount - 1][READ]) == ERROR) {
                            // perror("[CW] Middle dup error: replacing input\n");
                        }
                        close(STDOUT_FILENO);
                        if (dup(fds[processCount][WRITE]) == ERROR) {
                            // perror("[CW] Middle dup error: write into pipe\n");
                        }
                        // close(fds[processCount - 1][READ]);
                        // close(fds[processCount][WRITE]);
                        break;

                    case end:
                        close(fds[processCount - 1][WRITE]);
                        // if (dup2(fds[processCount - 1][READ], STDIN_FILENO) == ERROR) {
                        //     perror("[CW] End dup error: replacing input\n");
                        // }
                        close(STDIN_FILENO);
                        if (dup(fds[processCount - 1][READ]) == ERROR) {
                            // perror("[CW] End dup error: replacing input\n");
                        }
                        // close(fds[processCount - 1][READ]);
                        break;

                    default: perror("[CW] Scenario not defined\n"); break;
                }
                // fflush(stdout);
                if (execv(singleProcess[0], (char *const *) singleProcess) == ERROR) {
                    perror("[CW] Failed to run command\n");
                }
                break;
            // PARENT
            default:
                switch (scenario) {
                    case start: 
                    case middle:
                        if (close(fds[processCount][READ]) == ERROR) {
                            perror("Close Error Read");
                        }
                        if (close(fds[processCount][WRITE]) == ERROR) {
                            perror("Close Error Write");
                        };
                        break;

                    case end:
                        finalpid = pid;
                        char *finalpath;
                        finalpath = (char *) malloc(sizeof(char)*100);
                        strcpy(finalpath, path);

                        // initialise current status
                        sm_status_t currStatus;
                        currStatus.path = finalpath;
                        currStatus.pid = finalpid;
                        currStatus.running = true;

                        // save state
                        processArr[currProcess] = currStatus;
                        currProcess++;
                        break;

                    default: perror("[CW] Scenario not defined in parent\n"); break;
                }
                processCount++;
        }
    }
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
        // strcpy(statuses[i].path, processArr[i].path);
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
