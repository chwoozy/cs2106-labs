/*************************************
* Lab 4
* Name: Wong Choon Wei
* Student No: A0202190X
* Lab Group: 11
*************************************/

#include "mmf.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>


void *mmf_create_or_open(const char *name, size_t sz) {
    /* TODO */
    int fd;
    void *ptr = NULL;
    fd = open(name, O_RDWR | O_CREAT, 0644);
    if (fd > 0) {
        printf("Past fd\n");
        int ft = ftruncate(fd, sz);
        if (ft == 0) {
            printf("Past truncate\n");
            ptr = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 );
            if (ptr == MAP_FAILED) {
                perror("Failed to map memory");
            } else {
                printf("Past fd\n");
                return ptr;
            }
        } else {
            perror("Failed to truncate size");
        }
    } else {
        perror("Failed to create file");
    }
    return ptr;
}

void mmf_close(void *ptr, size_t sz) {
    /* TODO */
    int status = munmap (ptr, sz);
    if (status != 0) {
        perror("Failed to unmap memory");
    }
}
