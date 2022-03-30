#include "main.h"
#include <stdlib.h>

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

void main_args(int argc, char* argv[], struct main_data* data) {
    /* remove this or implement as feature
    if (argc != 6)
        TODO;
        printf("Nao colocou o numero argumentos certo, tente novamente \n");
        exit(0);
    */
   //Putting args in struct "data"
   data->max_ops = argv[1];
   data->buffers_size = argv[2];
   data->n_clients = argv[3];
   data->n_drivers = argv[4];
   data->n_restaurants = argv[5];
}