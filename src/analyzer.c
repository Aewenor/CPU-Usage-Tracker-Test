#include "libs.h"
#include "globals.h"
#include "analyzer.h"

void *Analyzer(void *thid){

   int prevIdle, idle, prevBusy, busy, prevTotal, total, totaldiff, idlediff;

   pthread_mutex_lock(&mutex); //enable mutex

   for(int i=0;i<MAXCPUS;i++) {
      prevIdle = oldCpuValues[i][3]+oldCpuValues[i][4]; //previous idle + iowait
      idle = cpuValues[i][3]+cpuValues[i][4]; //current idle + iowait

      prevBusy = oldCpuValues[i][0]+oldCpuValues[i][1]+oldCpuValues[i][2]+oldCpuValues[i][5]+oldCpuValues[i][6]+oldCpuValues[i][7]; //previous user + nice + system + irq + softirq + steal

      busy = cpuValues[i][0]+cpuValues[i][1]+cpuValues[i][2]+cpuValues[i][5]+cpuValues[i][6]+cpuValues[i][7]; //current user + nice + system + irq + softirq + steal

      prevTotal = prevBusy+prevIdle;
      total = busy+idle;

      totaldiff = total - prevTotal;
      idlediff = idle - prevIdle;

      percentValue[i] = ((double)(totaldiff - idlediff)/totaldiff)*100.0;
   }

   pthread_mutex_unlock(&mutex); //release mutex

   return thid;
}
