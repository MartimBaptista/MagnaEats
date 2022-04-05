#include "driver.h"

int execute_driver(int driver_id, struct communication_buffers* buffers, struct main_data* data) {
    int* counter = data->driver_stats;
    while (data->terminate == 0) {
        struct operation op;
        driver_receive_operation(&op, buffers, data);
        if(op.id != -1){
            driver_process_operation(&op, driver_id, data, counter);
            driver_send_answer(&op, buffers, data);
        }
    }
    return *counter;
}

void driver_receive_operation(struct operation* op, struct communication_buffers* buffers, struct main_data* data) {
    if (*(data->terminate) == 1)
        return;
    else
        read_rest_driver_buffer(buffers->rest_driv, data->buffers_size, op);
}

void driver_process_operation(struct operation* op, int driver_id, struct main_data* data, int* counter) {
    op->receiving_driver = driver_id;
    op->status = 'D';
    *counter++;
    data->results = op;
}

void driver_send_answer(struct operation* op, struct communication_buffers* buffers, struct main_data* data) {
    write_driver_client_buffer(buffers->driv_cli, data->buffers_size, op);
}