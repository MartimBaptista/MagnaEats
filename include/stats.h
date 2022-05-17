#ifndef STATS_H_GUARD
#define STATS_H_GUARD

#include "main.h"

//cria um novo ficheiro para estatísticas
void newStatsFile(char *filename);

//fecha o ficheiro de estatísticas
void closeStatsFile();

//imprime as estatisticas no ficheiro de estatísticas a partir das informações struct main_data* data
void statistics(struct main_data* data);


#endif