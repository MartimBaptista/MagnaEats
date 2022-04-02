#include "main.h"

#include "/home/martin/Documents/MagnaEats/MagnaEats/include/memory.h" //so that vs can dettect the erros TO REMOVE!!!!!!
#include "/home/martin/Documents/MagnaEats/MagnaEats/include/main.h"
#include "/home/martin/Documents/MagnaEats/MagnaEats/include/process.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    data->restaurant_pids = create_dynamic_memory(data->n_restaurants * sizeof(int));
    data->driver_pids = create_dynamic_memory(data->n_drivers * sizeof(int));
    data->client_pids = create_dynamic_memory(data->n_clients * sizeof(int));

    data->restaurant_stats = create_dynamic_memory(data->n_restaurants * sizeof(int));
    data->driver_stats = create_dynamic_memory(data->n_drivers * sizeof(int));
    data->client_stats = create_dynamic_memory(data->n_clients * sizeof(int));
}   

void create_shared_memory_buffers(struct main_data* data, struct communication_buffers* buffers){
    size_t op_buffer_size = data->buffers_size * sizeof(struct operation);
    //MAIN -> REST
    buffers->main_rest->ptrs = create_shared_memory(STR_SHM_MAIN_REST_PTR, data->buffers_size * sizeof(int));
    buffers->main_rest->buffer = create_shared_memory(STR_SHM_MAIN_REST_BUFFER, op_buffer_size);
    //REST -> DRIV
    buffers->rest_driv->ptrs = create_shared_memory(STR_SHM_REST_DRIVER_PTR, sizeof(struct pointers));
    buffers->rest_driv->buffer = create_shared_memory(STR_SHM_REST_DRIVER_BUFFER, op_buffer_size);
    //DRIV -> CLI
    buffers->driv_cli->ptrs = create_shared_memory(STR_SHM_DRIVER_CLIENT_PTR, data->buffers_size * sizeof(int));
    buffers->driv_cli->buffer = create_shared_memory(STR_SHM_DRIVER_CLIENT_BUFFER, op_buffer_size);
    //results and terminate
    data->results = create_shared_memory(STR_SHM_RESULTS, data->max_ops * sizeof(struct operation));
    data->terminate = create_shared_memory(STR_SHM_TERMINATE, sizeof(int));
}

void launch_processes(struct communication_buffers* buffers, struct main_data* data){
    for (size_t i = 0; i < data->n_restaurants; i++){
        data->restaurant_pids[i] = launch_restaurant(i, buffers, data);
    }
    for (size_t i = 0; i < data->n_drivers; i++){
        data->client_pids[i] = launch_client(i, buffers, data);
    }
    for (size_t i = 0; i < data->n_clients; i++){
        data->driver_pids[i] = launch_driver(i, buffers, data);
    }
}

void user_interaction(struct communication_buffers* buffers, struct main_data* data){
    return;
}
    /*
    char interaction[20];
    int um, dois;
    char prato[200];
    printf("O que deseja? escreva help para obter informacoes \n");
    scanf("%s %i %i %s", interaction, &um, &dois, prato);  // SHOULD BE OK IF THE LATTER ARE EMPTY
    if (strcmp("request", interaction) == 0) {
        create_request( int* op_counter , buffers, data);
    }
    else if (strcmp("status", interaction) == 0) {
        read_status(data);
        user_interaction(buffers, data);
    }
    else if (strcmp("stop", interaction) == 0) {
        stop_execution(data, buffers);
    }
    else if (strcmp("help", interaction) == 0) {
        printf("Existem os seguintes comandos: \n");
        printf("request - cria nova operacao, exemplo: request (id do cliente) (id do restaurante) (prato) \n");
        printf ("status - verifica o estado de uma operação, exemplo: status (id da operacao) \n"); 
        printf ("stop - termina a execucao do sistema MagnaEats \n");
        printf ("help - o que acabou de pedir (: \n");
        user_interaction(buffers, data);
    }
    else {
        // mau input try again
        printf("Input nao aceite, tente novamente \n");
        user_interaction(buffers, data);
    }
*/

void create_request(int* op_counter, struct communication_buffers* buffers, struct main_data* data){
    if(*op_counter < data->max_ops){
        struct operation *new_operation;
        new_operation->id = *op_counter;
        //TODO
    }
}

void read_status(struct main_data* data){
    return;
}

void stop_execution(struct main_data* data, struct communication_buffers* buffers){
    return;
}

void wait_processes(struct main_data* data){
    return;
}

void write_statistics(struct main_data* data){
    return;
}

void destroy_memory_buffers(struct main_data* data, struct communication_buffers* buffers){
    //Dynamica memory destruction
        destroy_dynamic_memory(data->restaurant_pids);
        destroy_dynamic_memory(data->driver_pids);
        destroy_dynamic_memory(data->client_pids);

        destroy_dynamic_memory(data->restaurant_stats);
        destroy_dynamic_memory(data->driver_stats);
        destroy_dynamic_memory(data->client_stats);

    //Shared memory destruction
        destroy_shared_memory(STR_SHM_MAIN_REST_BUFFER, buffers->main_rest->buffer, data->buffers_size * sizeof(buffers->main_rest->buffer));
        destroy_shared_memory(STR_SHM_MAIN_REST_PTR, buffers->main_rest->ptrs, data->buffers_size * sizeof(buffers->main_rest->ptrs));
        
        destroy_shared_memory(STR_SHM_REST_DRIVER_BUFFER, buffers->rest_driv->buffer, data->buffers_size * sizeof(buffers->rest_driv->buffer));
        destroy_shared_memory(STR_SHM_REST_DRIVER_PTR, buffers->rest_driv->ptrs, data->buffers_size * sizeof(buffers->rest_driv->ptrs));
        
        destroy_shared_memory(STR_SHM_DRIVER_CLIENT_BUFFER, buffers->driv_cli->buffer, data->buffers_size * sizeof(buffers->driv_cli->buffer));
        destroy_shared_memory(STR_SHM_DRIVER_CLIENT_PTR, buffers->driv_cli->ptrs, data->buffers_size * sizeof(buffers->driv_cli->ptrs));
        
        destroy_shared_memory(STR_SHM_RESULTS, data->results, data->buffers_size * sizeof(data->results));
        
        destroy_shared_memory(STR_SHM_TERMINATE, data->terminate, data->buffers_size * sizeof(data->terminate));

}

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