/* Sistemas Operativos 2021/2022. Projeto 1. Grupo SO-004
    Realizado por: Cosmin Trandafir Nº 57101
                   João Serafim     Nº 56376
                   Martim Baptista  Nº 56273
*/ 
#include "driver.h"

#include <stdlib.h>
#include <stdio.h>

int execute_driver(int driver_id, struct communication_buffers* buffers, struct main_data* data) {
    int counter = 0;
    while (*data->terminate == 0) {
        struct operation op;
        driver_receive_operation(&op, buffers, data);
        if(op.id != -1){
            printf("Motorista recebeu pedido!\n");
            driver_process_operation(&op, driver_id, data, &counter);
            driver_send_answer(&op, buffers, data);
        }
    }
    return counter;
}

void driver_receive_operation(struct operation* op, struct communication_buffers* buffers, struct main_data* data) {
    if (*data->terminate == 1)
        return;
    else
        read_rest_driver_buffer(buffers->rest_driv, data->buffers_size, op);
}

void driver_process_operation(struct operation* op, int driver_id, struct main_data* data, int* counter) {
    op->receiving_driver = driver_id;
    op->status = 'D';
    (*counter)++;
    data->results[op->id].receiving_driver = op->receiving_driver;
    data->results[op->id].status = op->status;
}

void driver_send_answer(struct operation* op, struct communication_buffers* buffers, struct main_data* data) {
    write_driver_client_buffer(buffers->driv_cli, data->buffers_size, op);
}