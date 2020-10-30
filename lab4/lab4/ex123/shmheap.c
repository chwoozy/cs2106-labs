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
            void* val = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            mem.size = len;
            mem.ptr = val;
            mem.addr = val;
            if (mem.ptr == MAP_FAILED) {
                perror("Error in mapping memory");
            } else {
                return mem;
            }
        }
    }
}

shmheap_memory_handle shmheap_connect(const char *name) {
    /* TODO */
    struct stat shm_info;
    int fd = shm_open(name, O_RDWR, 0644);
    if (fd == -1) {
        perror("Error in opening shared memory");
    } else {
        fstat(fd, &shm_info);
        shmheap_memory_handle mem;
        void* val = mmap(NULL, shm_info.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        mem.size = shm_info.st_size;
        mem.ptr = val;
        mem.addr = val;
        if (mem.ptr == MAP_FAILED) {
            perror("Error in mapping memory");
        } else {
            return mem;
        }
    }

}

void shmheap_disconnect(shmheap_memory_handle mem) {
    /* TODO */
    int status = munmap(mem.ptr, mem.size);
    if (status == -1) {
        perror("Error in disconnecting");
    }
}

void shmheap_destroy(const char *name, shmheap_memory_handle mem) {
    /* TODO */
    int status = munmap(mem.ptr, mem.size);
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
    mem.ptr += sz;
}

void shmheap_free(shmheap_memory_handle mem, void *ptr) {
    /* TODO */
}

shmheap_object_handle shmheap_ptr_to_handle(shmheap_memory_handle mem, void *ptr) {
    /* TODO */
    shmheap_object_handle obj;
    obj.ptr = ptr;
    return obj;
}

void *shmheap_handle_to_ptr(shmheap_memory_handle mem, shmheap_object_handle hdl) {
    /* TODO */
    return hdl.ptr;
}
