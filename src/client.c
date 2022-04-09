/* Sistemas Operativos 2021/2022. Projeto 1. Grupo SO-004
    Realizado por: Cosmin Trandafir Nº 57101
                   João Serafim     Nº 56376
                   Martim Baptista  Nº 56273
*/ 
#include "client.h"

#include <stdlib.h>
#include <stdio.h>

int execute_client(int client_id, struct communication_buffers* buffers, struct main_data* data){
    int counter = 0;
    while (*data->terminate == 0){
        struct operation op;
        client_get_operation(&op, client_id, buffers, data);
        if(op.id != -1){
            printf("Cliente recebeu pedido!\n");
            client_process_operation(&op, client_id, data, &counter);
        }
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
    (*counter)++;
    data->results[op->id] = *op;
}