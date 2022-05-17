#ifndef TIME_H_GUARD
#define TIME_H_GUARD

#include <time.h>

//resgister a timespec to the moment this was called
void register_timespec(struct timespec* time);

//transforma a timespec fornecida numa string com o formato Ano-Mês-Dia Hora:Minuto:Segundo.Milésimos de segundo
const char* time_toString(struct timespec* ts);

//calc the total time (for stats.h)
double calc_total(struct timespec* start, struct timespec* end);


#endif