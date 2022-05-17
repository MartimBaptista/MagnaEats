#ifndef SIGNAL_H_GUARD
#define SIGNAL_H_GUARD

#include "mesignal.h"
#include "main.h"

void alarm_stats();

void set_global_structs(struct main_data* data, struct communication_buffers* buffers, struct semaphores* sems);

void print_stats();

struct itimerval* get_alarm_time();

void set_alarm_time(int value);

void ctrlC_signal();

struct sigaction define_sigaction();

#endif