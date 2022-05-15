#ifndef LOG_H_GUARD
#define LOG_H_GUARD

#include "main.h"

#include <time.h>
//logging shit to a file

void newLogFile(char *filename);

void closeLogFile();

//adds log entry (if entry doesnt have argument then int must be -1)
void add_log(char* instruction, int argument);

#endif 