#ifndef GLOBALS_H
#define GLOBALS_H

#include "libs.h"

extern pthread_mutex_t mutex; //initialize mutex

extern int cpuValues[MAXCPUS][10]; //Storage for current cpu values
extern int oldCpuValues[MAXCPUS][10]; //Storage for previous cpu values
extern double percentValue[MAXCPUS]; //percentage usage of cpu

#endif