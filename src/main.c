#include "main.h"
#include "process.h"
#include "memory.h"
#include "restaurant.h"
#include "driver.h"
#include "client.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void main_args(int argc, char* argv[], struct main_data* data) {
    if (argc != 6){
        printf("Uso: magnaeats max_ops buffers_size n_restaurants n_drivers n_clients\n");
        printf("Exemplo: ./bin/magnaeats 10 10 1 1 1\n");
        exit(0);
    }
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
    buffers->rest_driv->ptrs = create_shared_memory(STR_SHM_REST_DRIVER_PTR, data->buffers_size * sizeof(struct pointers));
    buffers->rest_driv->buffer = create_shared_memory(STR_SHM_REST_DRIVER_BUFFER, op_buffer_size);
    //DRIV -> CLI
    buffers->driv_cli->ptrs = create_shared_memory(STR_SHM_DRIVER_CLIENT_PTR, data->buffers_size * sizeof(int));
    buffers->driv_cli->buffer = create_shared_memory(STR_SHM_DRIVER_CLIENT_BUFFER, op_buffer_size);
    //results and terminate
    data->results = create_shared_memory(STR_SHM_RESULTS, data->max_ops * sizeof(struct operation));
    data->terminate = create_shared_memory(STR_SHM_TERMINATE, sizeof(int));
    *data->terminate = 0;
    for (size_t i = 0; i < data->max_ops; i++){
        data->results[i].id = -1;
    }
    
}

void launch_processes(struct communication_buffers* buffers, struct main_data* data){
    for (size_t i = 0; i < data->n_restaurants; i++){
        data->restaurant_pids[i] = launch_restaurant(i, buffers, data);
    }
    for (size_t i = 0; i < data->n_drivers; i++){
        data->driver_pids[i] = launch_driver(i, buffers, data);
    }
    for (size_t i = 0; i < data->n_clients; i++){
        data->client_pids[i] = launch_client(i, buffers, data);
    }
}

void user_interaction(struct communication_buffers* buffers, struct main_data* data){
    char interaction[20];
    int counter;
    printf("Ações disponíveis:\n");
    printf("request (client) (restaurant) (dish) - Criar um novo pedido\n");
    printf("status (id) - Consultar o estado de um pedido\n");
    printf("stop - Termina a execução do magnaeats.\n");
    printf("help - Imprime informação sobre as ações disponíveis.\n");

    while (*data->terminate == 0){
        usleep(500);
        printf("Introduzir ação:\n");
        scanf("%s", interaction);
        if (strcmp("request", interaction) == 0) {
            create_request(&counter, buffers, data);
        }
        else if (strcmp("status", interaction) == 0) {
            read_status(data);
        }
        else if (strcmp("stop", interaction) == 0) {
            printf("Terminando o MAGNAEATS!\n");
            stop_execution(data, buffers);
        }
        else if (strcmp("help", interaction) == 0) {
            printf("Ações disponíveis:\n");
            printf("request (client) (restaurant) (dish) - Criar um novo pedido\n");
            printf("status (id) - Consultar o estado de um pedido\n");
            printf("stop - Termina a execução do magnaeats.\n");
            printf("help - Imprime informação sobre as ações disponíveis.\n");
        }
        else {
            printf("Ação não reconhecida, insira 'help' para assistência.\n");
        }
    }
}

void create_request(int* op_counter, struct communication_buffers* buffers, struct main_data* data){
    if(*op_counter < data->max_ops){
        struct operation new_operation;
        int client;
        int rest;
        //TODO
        char dish[20];
        //TODO
        scanf("%d %d %s", &client, &rest, dish);
        printf("Dish: %s\n", dish);
        new_operation.id = *op_counter;
        new_operation.requesting_client = client;
        new_operation.requested_rest = rest;
        //TODO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //printf("------------%p\n", new_operation.requested_dish);
        //strcpy(new_operation.requested_dish, dish);
        new_operation.requested_dish = dish;
        //printf("Dish??: %s\n", new_operation.requested_dish);
        //TOD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        new_operation.status = 'I';
        data->results[new_operation.id] = new_operation;
        write_main_rest_buffer(buffers->main_rest, data->buffers_size, &new_operation);
        (*op_counter)++;
        printf("O pedido #%d foi criado!\n", new_operation.id);
    }
    else
        printf("Numero maximo de operações alcançado!!!\n");
}

void read_status(struct main_data* data){
    int id;
    struct operation op;
    int found = 0;
    scanf("%d", &id);
    for (size_t i = 0; i < data->max_ops; i++){
        op = data->results[i];
        if(op.id == id && found == 0){
            found = 1;
            printf("Pedido %d com estado %c requisitado pelo cliente %d ao restaurante %d com o prato %s, ", op.id, op.status, op.requesting_client, op.requested_rest, op.requested_dish);
            if('I' == op.status)
                printf("ainda não foi recebido no restaurante!\n");
            else{
                printf("foi tratado pelo restaurante %d, ", op.receiving_rest);
                if('R' == op.status)
                    printf("ainda não foi recebido pelo motorista!\n");
                else{
                    printf("foi ecaminhado pelo motorista %d, ", op.receiving_driver);
                    if('D' == op.status)
                        printf("ainda não foi recebido pelo cliente!\n");
                    else{
                        printf("foi recebido pelo cliente %d.\n", op.receiving_client);
                    }
                }
            }
        }
    }
    if (found == 0)
        printf("O Pedido %d ainda não é válido!\n", id);
}

void stop_execution(struct main_data* data, struct communication_buffers* buffers){
    *data->terminate = 1;
    wait_processes(data);
    write_statistics(data);
    destroy_memory_buffers(data, buffers);
}

void wait_processes(struct main_data* data){
    for (size_t i = 0; i < data->n_restaurants; i++)
        data->restaurant_stats[i] = wait_process(data->restaurant_pids[i]);
    for (size_t i = 0; i < data->n_drivers; i++)
        data->driver_stats[i] = wait_process(data->driver_pids[i]);
    for (size_t i = 0; i < data->n_clients; i++)
        data->client_stats[i] = wait_process(data->client_pids[i]);
}

void write_statistics(struct main_data* data){
    printf("Imprimindo estatísticas:\n");
    for (size_t i = 0; i < data->n_restaurants; i++){
        printf("Restaurante %zu preparou %d pedidos!\n", i, data->restaurant_stats[i]);
    }
    for (size_t i = 0; i < data->n_drivers; i++){
        printf("Motorista %zu entregou %d pedidos!\n", i, data->driver_stats[i]);
    }
    for (size_t i = 0; i < data->n_clients; i++){
        printf("Cliente %zu recebeu %d pedidos!\n", i, data->client_stats[i]);
    }
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
        size_t op_buffer_size = data->buffers_size * sizeof(struct operation);
        //MAIN -> REST
        destroy_shared_memory(STR_SHM_MAIN_REST_BUFFER, buffers->main_rest->buffer, op_buffer_size);
        destroy_shared_memory(STR_SHM_MAIN_REST_PTR, buffers->main_rest->ptrs, data->buffers_size * sizeof(int));
        //REST -> DRIV
        destroy_shared_memory(STR_SHM_REST_DRIVER_BUFFER, buffers->rest_driv->buffer, op_buffer_size);
        destroy_shared_memory(STR_SHM_REST_DRIVER_PTR, buffers->rest_driv->ptrs, data->buffers_size * sizeof(struct pointers));
        //DRIV -> CLI
        destroy_shared_memory(STR_SHM_DRIVER_CLIENT_BUFFER, buffers->driv_cli->buffer, op_buffer_size);
        destroy_shared_memory(STR_SHM_DRIVER_CLIENT_PTR, buffers->driv_cli->ptrs, data->buffers_size * sizeof(int));
        //results and terminate
        destroy_shared_memory(STR_SHM_RESULTS, data->results, data->max_ops * sizeof(struct operation));

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
    destroy_shared_memory(STR_SHM_TERMINATE, data->terminate, sizeof(int));
    destroy_dynamic_memory(data); 
    destroy_dynamic_memory(buffers->main_rest); 
    destroy_dynamic_memory(buffers->rest_driv); 
    destroy_dynamic_memory(buffers->driv_cli); 
    destroy_dynamic_memory(buffers); 
} 