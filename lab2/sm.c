/**
 * CS2106 AY 20/21 Semester 1 - Lab 2
 * Name: Wong Choon Wei
 * Student No: A0202190X
 * Lab Group: 11
 *
 * This file contains function definitions. Your implementation should go in
 * this file.
 * 
 * 
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

sm_status_t processArr[SM_MAX_SERVICES][SM_MAX_SERVICES];
size_t currProcess;

// Use this function to any initialisation if you need to.
void sm_init(void) {
    currProcess = 0;

    for(int i = 0; i < SM_MAX_SERVICES; i++) {
        for (int j = 0; j < SM_MAX_SERVICES; j++) {
            sm_status_t dummyStatus = {-1, NULL, false};
            processArr[i][j] = dummyStatus;
        }
    }
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
        pid_t pid = fork();
        switch (pid) {
            case ERROR: 
                perror("[CW] Error in creating child processes\n"); 
                break;

            case CHILD:
                switch (scenario) {
                    case single: 
                        break;

                    case start:
                        close(fds[processCount][READ]);
                        if (dup2(fds[processCount][WRITE], STDOUT_FILENO) == ERROR) {
                            perror("[CW] Start dup error: write into pipe\n");
                        };
                        break;

                    case middle:
                        close(fds[processCount - 1][WRITE]);
                        if (dup2(fds[processCount - 1][READ], STDIN_FILENO) == ERROR) {
                            perror("[CW] Middle dup error: replacing input\n");
                        }
                        close(fds[processCount - 1][READ]);

                        close(fds[processCount][READ]);
                        if (dup2(fds[processCount][WRITE], STDOUT_FILENO) == ERROR) {
                            perror("[CW] Middle dup error: write into pipe\n");
                        }
                        close(fds[processCount][WRITE]);
                        break;

                    case end:
                        close(fds[processCount - 1][WRITE]);
                        if (dup2(fds[processCount - 1][READ], STDIN_FILENO) == ERROR) {
                            perror("[CW] End dup error: replacing input\n");
                        }
                        close(fds[processCount - 1][READ]);
                        break;

                    default: perror("[CW] Scenario not defined\n"); break;
                }

                for(int i = 0; i < cmdCount - 1; i++) {
                    close(fds[i][READ]);
                    close(fds[i][WRITE]);
                }
                
                if (execv(singleProcess[0], (char *const *) singleProcess) == ERROR) {
                    perror("[CW] Failed to run command\n");
                }
                break;

            // PARENT
            default:
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
                processArr[currProcess][processCount] = currStatus;
                processCount++;
                break;
        }
    }
    for (int i = 0; i < cmdCount - 1; i++) {
        close(fds[i][READ]);
        close(fds[i][WRITE]);
    }
    currProcess++;
}

// Exercise 1b: print service status
size_t sm_status(sm_status_t statuses[]) {
    int status;
    for(size_t i = 0; i < currProcess; i++) {
        for(size_t j = 0; j < SM_MAX_SERVICES; j++) {
            if ((processArr[i][j + 1]).path == NULL && (processArr[i][j + 1]).pid == ERROR) {
                if (waitpid(processArr[i][j].pid, &status, WNOHANG) != 0) {
                    processArr[i][j].running = false;
                } else {
                    processArr[i][j].running = true;
                }
                statuses[i] = processArr[i][j];
                break;
            }
        }
    }
    return currProcess;
}

// Exercise 3: stop service, wait on service, and shutdown
void sm_stop(size_t index) {
    int status;
    for (int j = 0; j < SM_MAX_SERVICES; j++) {
        sm_status_t proc = processArr[index][j];
        if (proc.path == NULL && proc.pid == ERROR) {
            break;
        } else {
            if (waitpid(proc.pid, &status, WNOHANG) == 0) {
                kill(proc.pid, SIGTERM);
            }
        }
    }
    sm_wait(index);
}

void sm_wait(size_t index) {
    int status;
    for (int j = 0; j < SM_MAX_SERVICES; j++) {
        sm_status_t proc = processArr[index][j];
        if (proc.path == NULL && proc.pid == ERROR) {
            break;
        } else {
            if (waitpid(proc.pid, &status, WNOHANG) == 0) {
                while (1) {
                    if (waitpid(proc.pid, &status, WNOHANG) != 0) {
                        break;
                    } else {
                        sleep(1);
                    }
                }
            }
        }
    }
}

void sm_shutdown(void) {
    for(size_t i = 0; i < currProcess; i++) {
        sm_stop(i);
    }
}

// Exercise 4: start with output redirection
void sm_startlog(const char *processes[]) {
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
        pid_t pid = fork();
        switch (pid) {
            case ERROR: 
                perror("[CW] Error in creating child processes\n"); 
                break;

            case CHILD:
                switch (scenario) {
                    case single:
                        break;

                    case start:
                        close(fds[processCount][READ]);
                        if (dup2(fds[processCount][WRITE], STDOUT_FILENO) == ERROR) {
                            perror("[CW] Start dup error: write into pipe\n");
                        };
                        break;

                    case middle:
                        close(fds[processCount - 1][WRITE]);
                        if (dup2(fds[processCount - 1][READ], STDIN_FILENO) == ERROR) {
                            perror("[CW] Middle dup error: replacing input\n");
                        }
                        close(fds[processCount - 1][READ]);

                        close(fds[processCount][READ]);
                        if (dup2(fds[processCount][WRITE], STDOUT_FILENO) == ERROR) {
                            perror("[CW] Middle dup error: write into pipe\n");
                        }
                        close(fds[processCount][WRITE]);
                        break;

                    case end:
                        close(fds[processCount - 1][WRITE]);
                        if (dup2(fds[processCount - 1][READ], STDIN_FILENO) == ERROR) {
                            perror("[CW] End dup error: replacing input\n");
                        }
                        close(fds[processCount - 1][READ]);
                        break;

                    default: perror("[CW] Scenario not defined\n"); break;
                }
                if (scenario == single || scenario == end) {
                    char num[10];
                    sprintf(num, "%zu", currProcess);
                    char filename[256] = "service";
                    strcat(filename, num);
                    strcat(filename, ".log");
                    freopen(filename, "a+", stdout);
                }

                for(int i = 0; i < cmdCount - 1; i++) {
                    close(fds[i][READ]);
                    close(fds[i][WRITE]);
                }
                
                if (execv(singleProcess[0], (char *const *) singleProcess) == ERROR) {
                    perror("[CW] Failed to run command\n");
                }
                break;

            // PARENT
            default:
                // }
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
                processArr[currProcess][processCount] = currStatus;
                processCount++;
                break;
        }
    }
    for (int i = 0; i < cmdCount - 1; i++) {
        close(fds[i][READ]);
        close(fds[i][WRITE]);
    }
    currProcess++;
}

// Exercise 5: show log file
void sm_showlog(size_t index) {
    char num[10];
    char x;
    sprintf(num, "%zu", index);
    char filename[100] = "service";
    strcat(filename, num);
    strcat(filename, ".log");
    
    if(access(filename, F_OK) != ERROR) {
        FILE *file;
        file = fopen(filename, "r");
        while((x = fgetc(file)) != EOF) {
            printf("%c", x);
        }
        fclose(file);
    } else {
        printf("service has no log file");
    }
}