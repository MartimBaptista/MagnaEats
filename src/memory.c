//#include "memory.h"
#include "/home/martin/Documents/MagnaEats/MagnaEats/include/memory.h" //so that vs can dettect the erros TO REMOVE!!!!!!
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void* create_shared_memory(char* name, int size){
    void* ptr;
    int ret;
    int fd = shm_open(name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1){
        perror("Failed to open shared memory");
        exit(1);
    }

    ret = ftruncate(fd, size);
    if (ret == -1)
    {
        perror("Failed to assign size to shared memory");
        exit(1);
    }

    ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED)
    {
        perror("Failed to map shared memory");
        exit(1);
    }

    return ptr;
}

void* create_dynamic_memory(int size){
    void* buffer = calloc(1, size);
    if (buffer == NULL){
        perror("Failed to create dynamic memory");
        exit(1);
    }
    return buffer;
}

void destroy_shared_memory(char* name, void* ptr, int size){
    if (munmap(ptr, size) == -1){
        perror("Failed to unmap shared memory");
        exit(1);
    }
    if (shm_unlink(name) == -1){
        perror("Failed to unlink shared memory");
        exit(1);
    }
}

void destroy_dynamic_memory(void* ptr){
    free(ptr);
    //TODO way to dettec fail
}

void write_main_rest_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op){
    struct operation* it = buffer->buffer;
    for (size_t i = 0; i < buffer_size; i++)
    {
        if (buffer->ptrs[i] == 0)
        {
            it = op;
            buffer->ptrs[i] = 1;
        }
        it++;
    }
}

//void write_rest_driver_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op);

//void write_driver_client_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op);

void read_main_rest_buffer(struct rnd_access_buffer* buffer, int rest_id, int buffer_size, struct operation* op){
    struct operation* it = buffer->buffer;
    for (size_t i = 0; i < buffer_size; i++)
    {
        if (buffer->ptrs[i] == 1 && it->receiving_rest == rest_id) //id->receiving_rest or requested_rest????? WTF IS THE DIFERENCE??? TODO!!!!
        {
            *op = *it;
            buffer->ptrs[i] = 0;
        }
        it++;
    }
}

//void read_rest_driver_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op);

//void read_driver_client_buffer(struct rnd_access_buffer* buffer, int client_id, int buffer_size, struct operation* op);
