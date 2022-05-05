/* Sistemas Operativos 2021/2022. Projeto 1. Grupo SO-004
    Realizado por: Cosmin Trandafir Nº 57101
                   João Serafim     Nº 56376
                   Martim Baptista  Nº 56273
*/
#include "process.h"
#include "restaurant.h"
#include "driver.h"
#include "client.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int launch_restaurant(int restaurant_id, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    int pid;
    pid = fork();

    if(pid == 0)
        exit(execute_restaurant(restaurant_id, buffers, data, sems));
    else
        return pid;
}

int launch_driver(int driver_id, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    int pid;
    pid = fork();

    if(pid == 0)
        exit(execute_driver(driver_id, buffers, data, sems));
    else
        return pid;
}

int launch_client(int client_id, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    int pid;
    pid = fork();
    if(pid == 0)
        exit(execute_client(client_id, buffers, data, sems));
    else
        return pid;
}

int wait_process(int process_id){
    int status;
    int pid = waitpid(process_id, &status , 0);
    if (WIFEXITED(status)){
        return WEXITSTATUS(status);
    }
    else{
        perror("Wait for process failed");
        exit(1);
    }
}