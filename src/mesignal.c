#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#include "mesignal.h"
#include "memory.h"
#include "main.h"

struct operation* main_results;
size_t max_operations;

struct itimerval timer_value;

void alarm_stats(){
    print_stats();
}

void set_results_stats(struct main_data* data){
    main_results = data->results;
    max_operations = data->max_ops;
}

void print_stats(){
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