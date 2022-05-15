#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LINE_SIZE 256
#define DIVISOR_NSEC 1000000

FILE *log_file;


void newLogFile(char *filename){
    log_file = fopen(filename, "w");
    if (log_file == NULL){
        perror("Log File"); exit(1);
    }
}

void closeLogFile(){
    fclose(log_file);
}

void add_log(char *instruction, int argument){
    char buffer1[LINE_SIZE];
    struct timespec ts;

    if( clock_gettime( CLOCK_REALTIME, &ts) == -1 ) {
    perror("clock gettime"); exit(1);
    }

    strftime(buffer1, LINE_SIZE, "%Y-%m-%d %H:%M:%S", localtime(&(ts.tv_sec)));

    

    if (argument == -1)
        fprintf(log_file, "%s.%ld %s\n", buffer1, ts.tv_nsec/DIVISOR_NSEC, instruction);
    else 
        fprintf(log_file, "%s.%ld %s %d\n", buffer1, ts.tv_nsec/DIVISOR_NSEC, instruction, argument);
    
}