/*************************************
* Lab 4
* Name:
* Student No:
* Lab Group:
*************************************/

#include "shmheap.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>


shmheap_memory_handle shmheap_create(const char *name, size_t len) {
    /* TODO */
    int fd = shm_open(name, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("Error in creating shared memory");
    } else {
        if (ftruncate(fd, len) == - 1) {
            perror("Error in truncating file");
        } else {
            shmheap_memory_handle mem;
            void* addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            shmheap_info* info = addr;
            info->count = 0;
            info->objects[0] = sizeof(shmheap_info);
            info->size = len;
            mem.addr = addr;
            if (addr == MAP_FAILED) {
                perror("Error in mapping memory");
            } else {
                return mem;
            }
        }
    }
}

shmheap_memory_handle shmheap_connect(const char *name) {
    /* TODO */
    struct stat s;
    int fd = shm_open(name, O_RDWR, 0644);
    if (fd == -1) {
        perror("Error in opening shared memory");
    } else {
        fstat(fd, &s);
        shmheap_memory_handle mem;
        void* addr = mmap(NULL, s.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        mem.addr = addr;
        if (addr == MAP_FAILED) {
            perror("Error in mapping memory");
        } else {
            return mem;
        }
    }

}

void shmheap_disconnect(shmheap_memory_handle mem) {
    /* TODO */
    shmheap_info *info = mem.addr;
    int status = munmap(mem.addr, info->size);
    if (status == -1) {
        perror("Error in disconnecting");
    }
}

void shmheap_destroy(const char *name, shmheap_memory_handle mem) {
    /* TODO */
    shmheap_info *info = mem.addr;
    int status = munmap(mem.addr, info->size);
    if (status == -1) {
        perror("Error in disconnecting");
    } else {
        shm_unlink(name);
    }
}

void *shmheap_underlying(shmheap_memory_handle mem) {
    /* TODO */
}

void *shmheap_alloc(shmheap_memory_handle mem, size_t sz) {
    /* TODO */
    shmheap_info *info = mem.addr;
    info->count++;
    info->objects[info->count] = sz + info->objects[info->count - 1];
    return mem.addr + info->objects[info->count - 1];
}

void shmheap_free(shmheap_memory_handle mem, void *ptr) {
    /* TODO */
}

shmheap_object_handle shmheap_ptr_to_handle(shmheap_memory_handle mem, void *ptr) {
    /* TODO */
    shmheap_object_handle obj;
    shmheap_info *info = mem.addr;
    obj.start = info->objects[0];
    return obj;
}

void *shmheap_handle_to_ptr(shmheap_memory_handle mem, shmheap_object_handle hdl) {
    /* TODO */
    return mem.addr + hdl.start;
}
