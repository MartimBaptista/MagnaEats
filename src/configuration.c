#include <stdio.h>
#include <configuration.h>
#include "memory.h"
#define LINE_SIZE 256

void configRead(char* argv[], struct main_data* data, struct configMagna* configM) {

    char linha[LINE_SIZE];
    char arg1[LINE_SIZE];
    // ugly
    int scanNum;

    FILE *configFile;
    configFile = fopen("config.txt", "r");
    if (configFile == NULL){
        perror("Ficheiro de dados.");
        exit(1);
    }
    while (fgets(linha, LINE_SIZE, configFile) != NULL){
        if (sscanf(linha, "%*s %s %*s", arg1) != 1){
            printf("erro");
            exit(2);
        }
        else {
            //ugly part TODO make it better
            if (scanNum == 0) {
                data->max_ops = arg1;
                scanNum++;
            }
            else if (scanNum == 1) {
                data->buffers_size = arg1;
                scanNum++;
            }
            else if (scanNum == 2) {
                data->n_restaurants = arg1;
                scanNum++;
            }
            else if (scanNum == 3) {
                data->n_drivers = arg1;
                scanNum++;
            }
            else if (scanNum == 4) {
                data->n_clients = arg1;
                scanNum++;
            }
            else if (scanNum == 5) {
                configM->log_filename = arg1;
                scanNum++;
            }
            else if (scanNum == 6) {
                configM->statistics_filename = arg1;
                scanNum++;
            }
            else if (scanNum == 7) {
                configM->alarm_time = atof(arg1);
                scanNum++;
            }
        }

    }
}