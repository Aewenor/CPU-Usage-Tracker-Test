#include "globals.h"
#include "printer.h"

void *Printer(void *thid){

   pthread_mutex_lock(&mutex); //enable mutex

   for(int i=0;i<MAXCPUS;i++) {
      //printf("Previous values CPU %d - User: %d Nice: %d System: %d Idle: %d iowait: %d irq: %d softirq: %d Steal: %d Guest: %d Guest_nice: %d \n", i, oldCpuValues[i][0], oldCpuValues[i][1],oldCpuValues[i][2],oldCpuValues[i][3],oldCpuValues[i][4],oldCpuValues[i][5],oldCpuValues[i][6],oldCpuValues[i][7],oldCpuValues[i][8],oldCpuValues[i][9]); //print previous values

      //printf("Current values CPU %d - User: %d Nice: %d System: %d Idle: %d iowait: %d irq: %d softirq: %d Steal: %d Guest: %d Guest_nice: %d \n", i, cpuValues[i][0], cpuValues[i][1],cpuValues[i][2],cpuValues[i][3],cpuValues[i][4],cpuValues[i][5],cpuValues[i][6],cpuValues[i][7],cpuValues[i][8],cpuValues[i][9]); //print current values

      printf("Current CPU %d usage: %.3lf %% \n", i, percentValue[i]); //print percentage values for CPU cores
   }

   pthread_mutex_unlock(&mutex); //release mutex

   return thid;
}
