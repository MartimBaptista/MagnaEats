#include "client.h"

#include <stdlib.h>
#include <stdio.h>

int execute_client(int client_id, struct communication_buffers* buffers, struct main_data* data){
    int counter;
    while (*data->terminate == 0){
        struct operation op;
        client_get_operation(&op, client_id, buffers, data);
        if(op.id != -1)
            client_process_operation(&op, client_id, data, &counter);
    }
    return counter;
}

void client_get_operation(struct operation* op, int client_id, struct communication_buffers* buffers, struct main_data* data){
    if(*data->terminate == 1)
        return;
    else
        read_driver_client_buffer(buffers->driv_cli, client_id, data->buffers_size, op);
}

void client_process_operation(struct operation* op, int client_id, struct main_data* data, int* counter){
    op->receiving_client = client_id;
    op->status = 'C';
    *counter++;
    data->results[op->id] = *op;
}