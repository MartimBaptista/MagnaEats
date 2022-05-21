/* Sistemas Operativos 2021/2022. Projeto parte 2. Grupo SO-004
    Realizado por: Cosmin Trandafir Nº 57101
                   João Serafim     Nº 56376
                   Martim Baptista  Nº 56273
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#include "mesignal.h"
#include "memory.h"
#include "main.h"

struct main_data* Data;
struct communication_buffers* Buffers;
struct semaphores* Sems;

struct itimerval timer_value;


void set_global_structs(struct main_data* data, struct communication_buffers* buffers, struct semaphores* sems){
    Data = data;
    Buffers = buffers;
    Sems = sems;
}


//--------------Alarm----------------------

void alarm_stats(){
    print_stats();
}

void print_stats(){
    struct operation* main_results = Data->results;
    size_t max_operations = Data->max_ops;;

    for (size_t i = 0; i < max_operations && main_results[i].id != -1; i++)
    {
        if(strcmp(&main_results[i].status, "C")==0){
            printf("request: %d status: %c start:%ld restaurante:%d rest_time:%ld motorista:%d driv_time:%ld cliente:%d end:%ld\n", 
            main_results[i].id, main_results[i].status, main_results[i].start_time.tv_sec, main_results[i].receiving_rest, main_results[i].rest_time.tv_sec, 
            main_results[i].receiving_driver, main_results[i].driver_time.tv_sec, main_results[i].receiving_client, main_results[i].client_end_time.tv_sec);        
        }else{
            printf("request: %d status: %c\n", main_results[i].id, main_results[i].status);
        }   
    }
}

struct itimerval* get_alarm_time(){
    return &timer_value;
}

void set_alarm_time(int value){
    timer_value.it_interval.tv_sec = value;
    timer_value.it_interval.tv_usec = 0;
    timer_value.it_value.tv_sec = value;
    timer_value.it_value.tv_usec = 0;
}

//---------------Signal-------------------

void ctrlC_signal(){
    stop_execution(Data, Buffers, Sems);
    usleep(1);
    
    //release memory before terminating 
    destroy_shared_memory(STR_SHM_TERMINATE, Data->terminate, sizeof(int));
    destroy_dynamic_memory(Data);
    destroy_dynamic_memory(Buffers->main_rest);
    destroy_dynamic_memory(Buffers->rest_driv);
    destroy_dynamic_memory(Buffers->driv_cli);
    destroy_dynamic_memory(Buffers);
    destroy_dynamic_memory(Sems->main_rest);
    destroy_dynamic_memory(Sems->rest_driv);
    destroy_dynamic_memory(Sems->driv_cli);
    destroy_dynamic_memory(Sems);

    usleep(1);
    kill(getpid(), SIGTERM);
}

struct sigaction define_sigaction(){
    struct sigaction sa;
    sa.sa_handler = ctrlC_signal;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    return sa;
}