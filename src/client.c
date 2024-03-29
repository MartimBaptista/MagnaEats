/* Sistemas Operativos 2021/2022. Projeto parte 2. Grupo SO-004
    Realizado por: Cosmin Trandafir Nº 57101
                   João Serafim     Nº 56376
                   Martim Baptista  Nº 56273
*/ 
#include "client.h"
#include "mesignal.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int execute_client(int client_id, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    int counter = 0;
    while (*data->terminate == 0){
        struct operation op;
        signal(SIGINT, SIG_IGN);
        client_get_operation(&op, client_id, buffers, data, sems);
        if(op.id != -1){
            printf("Cliente recebeu pedido!\n");
            client_process_operation(&op, client_id, data, &counter, sems);
        }
    }
    return counter;
}

void client_get_operation(struct operation* op, int client_id, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    if(*data->terminate == 1)
        return;
    else{
    consume_begin(sems->driv_cli);
    read_driver_client_buffer(buffers->driv_cli, client_id, data->buffers_size, op);
    consume_end(sems->driv_cli);
    }
}

void client_process_operation(struct operation* op, int client_id, struct main_data* data, int* counter, struct semaphores* sems){
    register_timespec(&(op->client_end_time));
    semaphore_mutex_lock(sems->results_mutex);
    op->receiving_client = client_id;
    op->status = 'C';
    (*counter)++;
    data->results[op->id].receiving_client = op->receiving_client;
    data->results[op->id].status = op->status;
    data->results[op->id].client_end_time = op->client_end_time;
    semaphore_mutex_unlock(sems->results_mutex);
}