#include "main.h"

#define FILENAME_SIZE (256)

struct configMagna { 
    int max_ops;			//número máximo de operações
	int buffers_size;		//tamanho máximo dos buffers de mem. partilhada

	int n_restaurants;		//número de restaurantes
	int n_drivers;			//número de motoristas
	int n_clients;			//número de clientes

    char log_filename[FILENAME_SIZE];                //nome do ficheiro de log
    char statistics_filename[FILENAME_SIZE];         //nome do ficheiro de estatísticas
    double alarm_time;                  //temporização para o alarme

};

void configRead(char* argv[], struct main_data* data);