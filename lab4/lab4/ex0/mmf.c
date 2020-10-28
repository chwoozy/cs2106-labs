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
    char *ptr = NULL;
    fd = open(name, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd > 0) {
        int ft = ftruncate(fd, sz);
        if (ft != -1) {
            ptr = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, fd, 0 );
            if (ptr != MAP_FAILED) {
                return ptr;
            } else {
                perror("Failed to map memory");
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
