#include "restaurant.h"

#include <stdlib.h>
#include <stdio.h>

int execute_restaurant(int rest_id, struct communication_buffers* buffers, struct main_data* data){
    int* counter = data->restaurant_stats;
    while (data->terminate == 0){
        struct operation *op;
        restaurant_receive_operation(op, rest_id, buffers, data);
        if(op->id != -1){
            restaurant_process_operation(op, rest_id, data, counter);
            restaurant_forward_operation(op, buffers, data);
        }
    }
    return *counter;
}




void restaurant_receive_operation(struct operation* op, int rest_id, struct communication_buffers* buffers, struct main_data* data){
    if(*data->terminate == 1)
        return;
    else
        read_main_rest_buffer(buffers->main_rest, rest_id, data->buffers_size, op);
}


void restaurant_process_operation(struct operation* op, int rest_id, struct main_data* data, int* counter){
    op->receiving_rest = rest_id;
    op->status = 'R';
    *counter++;
    data->results = op;
}


void restaurant_forward_operation(struct operation* op, struct communication_buffers* buffers, struct main_data* data){
    write_rest_driver_buffer(buffers->rest_driv, data->buffers_size, op);
}


