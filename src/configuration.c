#include "configuration.h"
#include "memory.h"
#include "mesignal.h"
#include "stats.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LINE_SIZE 256

void configRead(char* argv[], struct main_data* data) {

    char linha[LINE_SIZE];
    struct files f;

    FILE *configFile;
    configFile = fopen("config.txt", "r");
    if (configFile == NULL){
        perror("Ficheiro de dados.");
        exit(1);
    }
    fgets(linha, LINE_SIZE, configFile);
    sscanf(linha, "%*s %d %*s", &data->max_ops);
    fgets(linha, LINE_SIZE, configFile);
    sscanf(linha, "%*s %d %*s", &data->buffers_size);
    fgets(linha, LINE_SIZE, configFile);
    sscanf(linha, "%*s %d %*s", &data->n_restaurants);
    fgets(linha, LINE_SIZE, configFile);
    sscanf(linha, "%*s %d %*s", &data->n_drivers);
    fgets(linha, LINE_SIZE, configFile);
    sscanf(linha, "%*s %d %*s", &data->n_clients);
    
    //Log_file
    fgets(linha, LINE_SIZE, configFile);
    sscanf(linha, "%*s %s %*s", f.log_filename);
    newLogFile(f.log_filename);

    //Stats_file
    fgets(linha, LINE_SIZE, configFile);
    sscanf(linha, "%*s %s %*s", f.statistics_filename);
    newStatsFile(f.statistics_filename);
    
    
    //Alarm_time
    fgets(linha, LINE_SIZE, configFile);
    sscanf(linha, "%*s %d %*s", &f.alarm_time);
    set_alarm_time(f.alarm_time);

    fclose(configFile);
}