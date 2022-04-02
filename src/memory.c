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
    for (size_t i = 0; i < buffer_size; i++)
    {
        if (buffer->ptrs[i] == 0)
        {
            buffer->buffer[i] = *op;
            buffer->ptrs[i] = 1;
            break;
        }
    }
}

void write_rest_driver_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op){
    if ((buffer->ptrs->in + 1) % buffer_size != buffer->ptrs->out)
    {
        buffer->buffer[buffer->ptrs->in] = *op;
        buffer->ptrs->in = (buffer->ptrs->in + 1) % buffer_size;
    }
    
}

void write_driver_client_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op){
    for (size_t i = 0; i < buffer_size; i++)
    {
        if (buffer->ptrs[i] == 0)
        {
            buffer->buffer[i] = *op;
            buffer->ptrs[i] = 1;
            break;
        }
    }
}

void read_main_rest_buffer(struct rnd_access_buffer* buffer, int rest_id, int buffer_size, struct operation* op){
    op->id = -1;
    for (size_t i = 0; i < buffer_size; i++)
    {
        if (buffer->ptrs[i] == 1 && (buffer->buffer + 1)->receiving_rest == rest_id) //id->receiving_rest or requested_rest????? WTF IS THE DIFERENCE??? TODO!!!!
        {
            *op = buffer->buffer[i];
            buffer->ptrs[i] = 0;
            break;
        }
    }
}

void read_rest_driver_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op){
    if(buffer->ptrs->in != buffer->ptrs->out){
        *op = buffer->buffer[buffer->ptrs->out];
        buffer->ptrs->out = (buffer->ptrs->out + 1) % buffer_size;
    }
}

void read_driver_client_buffer(struct rnd_access_buffer* buffer, int client_id, int buffer_size, struct operation* op){
    op->id = -1;
    for (size_t i = 0; i < buffer_size; i++)
    {
        if (buffer->ptrs[i] == 1 && (buffer->buffer + 1)->receiving_client == client_id) //id->receiving_client or requested_client????? WTF IS THE DIFERENCE??? TODO!!!!
        {
            *op = buffer->buffer[i];
            buffer->ptrs[i] = 0;
            break;
        }
    }
}
