#ifndef CONFIG_H_GUARD
#define CONFIG_H_GUARD

#include "main.h"
#include "log.h"

struct files{

    char log_filename[256];           //nome do ficheiro de log
    char statistics_filename[256];    //nome do ficheiro de estatísticas
    int alarm_time;                //temporização para o alarme
};


void configRead(char* argv[], struct main_data* data);

#endif