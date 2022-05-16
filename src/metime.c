#include "metime.h"
#include <stdio.h>
#include <stdlib.h>

void register_timespec(struct timespec* time){
    if( clock_gettime( CLOCK_REALTIME, time) == -1 ) {
        perror("clock gettime"); exit(1);
    }
}

void calc_total(struct timespec* start, struct timespec* end){
    return;
}