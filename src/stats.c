#include "stats.h"
#include "metime.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

FILE* stats_file;

void newStatsFile(char *filename){
    stats_file = fopen(filename, "w");
    if (stats_file == NULL){
        perror("Stats File"); exit(1);
    }
}

void closeStatsFile(){
    fclose(stats_file);
}

void statistics(struct main_data* data){
    fprintf(stats_file, "Process Statistics:\n\n");

    for (size_t i = 0; i < data->n_restaurants; i++){
        fprintf(stats_file, "\tRestaurant %ld prepared %d requests!\n", i, data->restaurant_stats[i]);}
    for (size_t i = 0; i < data->n_drivers; i++){
        fprintf(stats_file, "\tDriver %ld delivered %d requests!\n", i, data->driver_stats[i]);}
    for (size_t i = 0; i < data->n_clients; i++){
        fprintf(stats_file, "\tClient %ld received %d requests!\n", i, data->client_stats[i]);}

    fprintf(stats_file, "\nRequest Statistics:\n");

    for (size_t i = 0; i < data->max_ops && data->results[i].id != -1; i++)
    {
        fprintf(stats_file, "\nRequest: %d\nStatus: %c\nRestaurant id: %d\nDriver id: %d\nClient id: %d\n"
        ,data->results[i].id, data->results[i].status, data->results[i].receiving_rest, data->results[i].receiving_driver
        ,data->results[i].receiving_client);

        fprintf(stats_file, "Created: %s\n", time_toString(&data->results[i].start_time));
        fprintf(stats_file, "Restaurant time: %s\n", time_toString(&data->results[i].rest_time));
        fprintf(stats_file, "Driver time: %s\n", time_toString(&data->results[i].driver_time));
        fprintf(stats_file, "Client time (end): %s\n", time_toString(&data->results[i].client_end_time));
        fprintf(stats_file, "Total Time:%.3f\n", calc_total(&data->results[i].start_time, &data->results[i].client_end_time));
    }
}