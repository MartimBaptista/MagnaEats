/* Sistemas Operativos 2021/2022. Projeto 1. Grupo SO-004
    Realizado por: Cosmin Trandafir Nº 57101
                   João Serafim     Nº 56376
                   Martim Baptista  Nº 56273
*/ 
#include "driver.h"

#include <stdlib.h>
#include <stdio.h>

int execute_driver(int driver_id, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    int counter = 0;
    while (*data->terminate == 0) {
        struct operation op;
        //SECCAO CRITICA
        driver_receive_operation(&op, buffers, data, sems);
        if(op.id != -1){
            printf("Motorista recebeu pedido!\n");
            driver_process_operation(&op, driver_id, data, &counter, sems);
            driver_send_answer(&op, buffers, data, sems);
        }
        //----------------------
    }
    return counter;
}

void driver_receive_operation(struct operation* op, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    if (*data->terminate == 1)
        return;
    else{
    consume_begin(sems->rest_driv);
    //SECCAO CRITICA
        read_rest_driver_buffer(buffers->rest_driv, data->buffers_size, op);
    //------------
    consume_end(sems->rest_driv);
    }
    
}

void driver_process_operation(struct operation* op, int driver_id, struct main_data* data, int* counter, struct semaphores* sems){
    produce_begin(sems->rest_driv);
    //SECCAO CRITICA
    op->receiving_driver = driver_id;
    op->status = 'D';
    (*counter)++;
    data->results[op->id].receiving_driver = op->receiving_driver;
    data->results[op->id].status = op->status;
    //----------------
    produce_end(sems->rest_driv);
}

void driver_send_answer(struct operation* op, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    produce_begin(sems->driv_cli);
    //SECCAO CRITICA
    write_driver_client_buffer(buffers->driv_cli, data->buffers_size, op);
    //-------------------
    produce_end(sems->driv_cli);
}