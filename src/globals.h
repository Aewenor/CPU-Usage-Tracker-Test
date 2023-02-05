#ifndef GLOBALS_H
#define GLOBALS_H

extern volatile sig_atomic_t sgnl; //for SIGTERM handling

extern pthread_mutex_t mutex; //initialize mutex
extern pthread_mutex_t logmutex; //initialize logger mutex

extern int cpuValues[MAXCPUS][10]; //Storage for current cpu values
extern int oldCpuValues[MAXCPUS][10]; //Storage for previous cpu values
extern double percentValue[MAXCPUS]; //percentage usage of cpu

extern char logmess[200]; 

#endif
