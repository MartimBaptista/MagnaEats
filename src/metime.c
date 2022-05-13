#include "metime.h"

void register_timespec(struct timespec* time){
    clock_gettime(CLOCK_REALTIME, time);
}

void calc_total(struct timespec* start, struct timespec* end){
    return;
}