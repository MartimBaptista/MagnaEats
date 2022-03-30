#include "main.h"
#include <stdlib.h>

void main_args(int argc, char* argv[], struct main_data* data) {
    /* remove this or implement as feature
    if (argc != 6)
        TODO?
        printf("Nao colocou o numero argumentos certo, tente novamente \n");
        exit(0);
    */
   //Putting args in struct "data"
   data->max_ops = atoi(argv[1]);
   data->buffers_size = atoi(argv[2]);
   data->n_clients = atoi(argv[3]);
   data->n_drivers = atoi(argv[4]);
   data->n_restaurants = atoi(argv[5]);
}

void create_dynamic_memory_buffers(struct main_data* data) {
    data->restaurant_pids = create_dynamic_memory(data->n_restaurants);
    data->driver_pids = create_dynamic_memory(data->n_drivers);
    data->client_pids = create_dynamic_memory(data->n_clients);

    data->restaurant_stats = create_dynamic_memory(data->n_restaurants);
    data->driver_stats = create_dynamic_memory(data->n_drivers);
    data->client_stats = create_dynamic_memory(data->n_clients);
}
/*
void create_shared_memory_buffers(struct main_data* data, struct communication_buffers* buffers) {
    //provavelmente n é nada disto
    char mainRest = "mRestaurante";
    strcat(mainRest, getuid());
    buffers->main_rest = create_shared_memory(mainRest,)
}
*/
//void launch_processes(struct communication_buffers* buffers, struct main_data* data)

//void user_interaction(struct communication_buffers* buffers, struct main_data* data);

//void create_request(int* op_counter, struct communication_buffers* buffers, struct main_data* data);

//void read_status(struct main_data* data);

//void stop_execution(struct main_data* data, struct communication_buffers* buffers);

//void wait_processes(struct main_data* data);

//void write_statistics(struct main_data* data);

//void destroy_memory_buffers(struct main_data* data, struct communication_buffers* buffers);

int main(int argc, char *argv[]) { 
    //init data structures 
    struct main_data* data = create_dynamic_memory(sizeof(struct main_data)); 
    struct communication_buffers* buffers = create_dynamic_memory(sizeof(struct communication_buffers)); 
    buffers->main_rest = create_dynamic_memory(sizeof(struct rnd_access_buffer)); 
    buffers->rest_driv = create_dynamic_memory(sizeof(struct circular_buffer)); 
    buffers->driv_cli = create_dynamic_memory(sizeof(struct rnd_access_buffer)); 
 
    //execute main code 
    main_args(argc, argv, data); 
    create_dynamic_memory_buffers(data); 
    create_shared_memory_buffers(data, buffers); 
    launch_processes(buffers, data); 
    user_interaction(buffers, data); 
    
    //release memory before terminating 
    destroy_dynamic_memory(data); 
    destroy_dynamic_memory(buffers->main_rest); 
    destroy_dynamic_memory(buffers->rest_driv); 
    destroy_dynamic_memory(buffers->driv_cli); 
    destroy_dynamic_memory(buffers); 
} 