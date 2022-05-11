#include "main.h"

#define FILENAME_SIZE (256)

struct configMagna { 

    char log_filename[FILENAME_SIZE];                //nome do ficheiro de log
    char statistics_filename[FILENAME_SIZE];         //nome do ficheiro de estatísticas
    double alarm_time;                  //temporização para o alarme

};

void configRead(char* argv[], struct main_data* data, struct configMagna* configM);