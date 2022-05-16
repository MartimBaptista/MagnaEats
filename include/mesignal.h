#ifndef SIGNAL_H_GUARD
#define SIGNAL_H_GUARD

#include "mesignal.h"
#include "main.h"

void alarm_stats();

void set_results_stats(struct main_data* data);

void print_stats();

struct itimerval* get_alarm_time();

void set_alarm_time(int value);

#endif