#include "configuration.h"
#include "memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LINE_SIZE 256

void configRead(char* argv[], struct main_data* data) {

    char linha[LINE_SIZE];

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
    fgets(linha, LINE_SIZE, configFile);
    sscanf(linha, "%*s %s %*s", data->log_filename);
    fgets(linha, LINE_SIZE, configFile);
    sscanf(linha, "%*s %s %*s", data->statistics_filename);
    fgets(linha, LINE_SIZE, configFile);
    sscanf(linha, "%*s %lf %*s", &data->alarm_time);
}