#include "metime.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 256
char buffer[LINE_SIZE];

void register_timespec(struct timespec* time){
    if( clock_gettime( CLOCK_REALTIME, time) == -1 ) {
        perror("clock gettime"); exit(1);
    }
}

const char* time_toString(struct timespec* ts){
    char nano[10];
    strftime(buffer, LINE_SIZE, "%Y-%m-%d %H:%M:%S", localtime(&(ts->tv_sec)));
    sprintf(nano, ".%ld", ts->tv_nsec/1000000);
    strcat(buffer, nano);
    return buffer;
}

double calc_total(struct timespec* start, struct timespec* end){
    return (end->tv_sec-start->tv_sec) + (end->tv_nsec-start->tv_nsec)/1000000000.0;
}