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

#define ROOT 16
#define NODE 8

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

            // mem init
            mem.addr = addr;
            mem.mmsize = len;

            // root init
            shmheap_root *root = addr;
            root->size = ROOT;
            root->count = 1;
            root->curr = '0';
            root->next = len;

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
        mem.mmsize = s.st_size;
        if (addr == MAP_FAILED) {
            perror("Error in mapping memory");
        } else {
            return mem;
        }
    }

}

void shmheap_disconnect(shmheap_memory_handle mem) {
    /* TODO */
    int status = munmap(mem.addr, mem.mmsize);
    if (status == -1) {
        perror("Error in disconnecting");
    }
}

void shmheap_destroy(const char *name, shmheap_memory_handle mem) {
    /* TODO */
    // shmheap_info *info = mem.addr;
    int status = munmap(mem.addr, mem.mmsize);
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
    
    // round to 8 bytes
    sz = (sz + 7) & (-8);

    shmheap_root *root = mem.addr;
    void* allocated = mem.addr;
    if (root->count == 1) {
        int freespace = root->next - sz - ROOT;
        // update root
        root->count++;
        root->curr = '1'; // assume that can fit into first space
        root->next = ROOT + sz;
        //root->next placeholder

        // update next free node
        shmheap_node *node = root + root->next;
        node->curr = '0';
        node->size = NODE;
        node->prev = sz + ROOT;
        node->next = freespace;

        allocated += ROOT;
    } else {
        if (root->curr == '0' && root->next >= sz) { // free and unused
                int freespace = root->next - ROOT - sz;
                allocated += ROOT;
                root->curr = '1';
            if (freespace > 8) { // perfect fit or have more space but not enough for bk
                root->next = sz + ROOT;
                
                // update node
                shmheap_node *node = allocated + sz;
                node->curr = '0';
                node->size = NODE;
                node->prev = sz + ROOT;
                node->next = freespace;

                // update next
                shmheap_node *next = node + node->next;
                next->prev = node->next;
            }
        } else {
            allocated += root->next;
            for (int i = 1; i < root->count; i++) {
                shmheap_node *node = allocated;
                if (node->curr == '0' && node->next >= sz) {
                    int freespace = node->next - NODE - sz;
                    allocated += NODE;
                    node->curr = '1';
                    if (freespace > 8)  {
                        node->next = sz + NODE;

                        //update next node
                        shmheap_node *nextnode = allocated + sz;
                        nextnode->curr = '0';
                        nextnode->size = NODE;
                        nextnode->prev = sz + NODE;
                        nextnode->next = freespace;

                        //update available next next node
                        if (i != (root->count - 1)) {
                            shmheap_node *next = nextnode + nextnode->next;
                            next->prev = nextnode->next;
                        }
                    }

                } else {
                    allocated += node->next; //may break if cant find free space, assume always got enough space
                }
            }
        }
        

    }
    return allocated; //may break if cant find free space, assume always got enough space
}

void shmheap_free(shmheap_memory_handle mem, void *ptr) {
    /* TODO */
    void* curr = mem.addr;
    shmheap_root *root = mem.addr;

    if (curr + root->size == ptr) {
        shmheap_node *next = curr + root->next;
        root->curr = '0';
        if (next->curr == '0') {
            root->next += next->next;
            root->count--;
        }
    } else {
        curr += root->next;
        
        for (int i = 1; i < root->count; i++) {
            shmheap_node *node = curr;
            if (curr + node->size == ptr) {
                shmheap_node *next = curr + node->next;
                shmheap_node *prev = curr - node->prev;
                node->curr = '0';
                if (i != (root->count - 1) && next->curr == '0') {
                    if (prev->curr == '0') {
                        node->next += next->next;
                        prev->next += node->next;
                        root->count -= 2;

                        if ((i + 2) < root->count) {
                            shmheap_node *evennext = prev + prev->next;
                            evennext->prev = prev->next;
                        }

                    } else {
                        node->next += next->next;
                        root->count--;
                    }
                } else {
                    if (prev->curr == '0') {
                        prev->next += node->next;
                        root->count--;

                        if(i != (root->count - 1)) {
                            next->prev = prev->next;
                        }

                    }
                }
                curr += node->next;
            } else {
                curr += node->next;
            }
        }
    }

    //does not handle if cant find ptr (assume that ptr is valid)
    
}

shmheap_object_handle shmheap_ptr_to_handle(shmheap_memory_handle mem, void *ptr) {
    /* TODO */
    // shmheap_object_handle obj;
    // shmheap_info *info = mem.addr;
    // obj.ptr = ptr;
    // return obj;
    int buffer = ptr - mem.addr;
    shmheap_object_handle obj;
    obj.ptr = buffer;
    return obj;
}

void *shmheap_handle_to_ptr(shmheap_memory_handle mem, shmheap_object_handle hdl) {
    /* TODO */
    // shmheap_info *info = mem.addr;
    // return mem.addr + info->start;
    return mem.addr + hdl.ptr;
}
