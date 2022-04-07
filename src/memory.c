#include "memory.h"

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
    if (ret == -1){
        perror("Failed to assign size to shared memory");
        exit(1);
    }

    ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED){
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
    for (size_t i = 0; i < buffer_size; i++){
        if (buffer->ptrs[i] == 0){
            buffer->buffer[i] = *op;
            buffer->ptrs[i] = 1;
            printf("\n Buffer main rest %d %d %c\n", buffer->buffer[i].requesting_client, buffer->buffer[i].requested_rest, buffer->buffer[i].status);;
            break;
        }
    }
}

void write_rest_driver_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op){
    if ((buffer->ptrs->in + 1) % buffer_size != buffer->ptrs->out){
        buffer->buffer[buffer->ptrs->in] = *op;
        buffer->ptrs->in = (buffer->ptrs->in + 1) % buffer_size;
        printf("\n operation %d %d ", op->requested_rest, op->receiving_rest);
        printf("\n in: %d out %d", buffer->ptrs->in, buffer->ptrs->out);
        printf("\n (out)Buffer rest driv %d %d\n",buffer->buffer[buffer->ptrs->out].id , buffer->buffer[buffer->ptrs->out].receiving_rest);
        printf("\n (in)Buffer rest driv %d %d\n",buffer->buffer[buffer->ptrs->in].id , buffer->buffer[buffer->ptrs->in].receiving_rest);
    }
}

void write_driver_client_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op){
    for (size_t i = 0; i < buffer_size; i++){
        if (buffer->ptrs[i] == 0){
            buffer->buffer[i] = *op;
            buffer->ptrs[i] = 1;
            printf("\n Buffer driv client %d %d\n", buffer->buffer[i].requesting_client, buffer->buffer[i].receiving_driver);
            break;
        }
    }
}

void read_main_rest_buffer(struct rnd_access_buffer* buffer, int rest_id, int buffer_size, struct operation* op){
    op->id = -1;
    for (size_t i = 0; i < buffer_size; i++){
        if (buffer->ptrs[i] == 1 && buffer->buffer->requested_rest == rest_id){
            *op = buffer->buffer[i];
            buffer->ptrs[i] = 0;
            printf("\nreq rest buff %d",buffer->buffer->requested_rest);
            printf("\n READ MR %d %d %d %c\n", op->id, op->requested_rest, op->receiving_rest, op->status );
            break;
        }
    }
}

void read_rest_driver_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op){
    op->id = -1;
    if(buffer->ptrs->in != buffer->ptrs->out){
        *op = buffer->buffer[buffer->ptrs->out];
        buffer->ptrs->out = (buffer->ptrs->out + 1) % buffer_size;
        printf("\n READ RD %d %d %d %d %c\n", op->id, op->receiving_client, op->receiving_driver, op->receiving_rest, op->status );
    }
}

void read_driver_client_buffer(struct rnd_access_buffer* buffer, int client_id, int buffer_size, struct operation* op){
    op->id = -1;
    for (size_t i = 0; i < buffer_size; i++){
        if (buffer->ptrs[i] == 1 && buffer->buffer->requesting_client == client_id){
            *op = buffer->buffer[i];
            buffer->ptrs[i] = 0;
            printf("\n READ DC %d %d %d %d %c\n", op->id, op->requesting_client, op->receiving_client, client_id, op->status );
            break;
        }
    }
}
