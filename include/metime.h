#ifndef TIME_H_GUARD
#define TIME_H_GUARD

#include <time.h>

//resgister a timespec to the moment this was called
void register_timespec(struct timespec time);

//calc the total time (for stats.h)
void calc_total(struct timespec* start, struct timespec* end);


#endif