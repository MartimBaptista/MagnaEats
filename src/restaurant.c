/* Sistemas Operativos 2021/2022. Projeto 1. Grupo SO-004
    Realizado por: Cosmin Trandafir Nº 57101
                   João Serafim     Nº 56376
                   Martim Baptista  Nº 56273
*/
#include "restaurant.h"

#include <stdlib.h>
#include <stdio.h>

int execute_restaurant(int rest_id, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    int counter = 0;
    while (*data->terminate == 0){
        struct operation op;
        restaurant_receive_operation(&op, rest_id, buffers, data, sems);
        if(op.id != -1){
            printf("Restaurante recebeu pedido!\n");
            restaurant_process_operation(&op, rest_id, data, &counter, sems);
            restaurant_forward_operation(&op, buffers, data, sems);
        }
    }
    return counter;
}

void restaurant_receive_operation(struct operation* op, int rest_id, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    if(*data->terminate == 1)
        return;
    else{
        consume_begin(sems->main_rest);
        read_main_rest_buffer(buffers->main_rest, rest_id, data->buffers_size, op);
        consume_end(sems->main_rest);
    }
}

void restaurant_process_operation(struct operation* op, int rest_id, struct main_data* data, int* counter, struct semaphores* sems){
    //marcar tempo
    register_timespec(op->rest_time);
    semaphore_mutex_lock(sems->results_mutex);
    op->receiving_rest = rest_id;
    op->status = 'R';
    (*counter)++;
    data->results[op->id].receiving_rest = op->receiving_rest;
    data->results[op->id].status = op->status;
    data->results[op->id].rest_time = op->rest_time;
    semaphore_mutex_unlock(sems->results_mutex);
}

void restaurant_forward_operation(struct operation* op, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    produce_begin(sems->rest_driv);
    write_rest_driver_buffer(buffers->rest_driv, data->buffers_size, op);
    produce_end(sems->rest_driv);
}