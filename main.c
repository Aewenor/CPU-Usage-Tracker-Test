#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define MAXCPUS 6

pthread_t thread_ids[5]; //IDs of threads

int cpuValues[MAXCPUS][10]; //Storage for current cpu values
int oldCpuValues[MAXCPUS][10]; //Storage for previous cpu values
double percentValue[MAXCPUS]; //percentage usage of cpu

void *Reader(void *thid){

   char string[100];
   char* token;
   int tokorder;
   FILE* fp;

for(int z=0;z<2;z++){

   memcpy(oldCpuValues,cpuValues, sizeof(cpuValues));

   fp = fopen("/proc/stat","r"); //open file
   fgets(string,100,fp); //skip first line with summed cpu usage
   for(int i=0;i<MAXCPUS;i++) {
      fgets(string,100,fp);
      token = strtok(string," ");
      tokorder=0;
      while(token!=NULL) {
         //if(tokorder!=0) oldCpuValues[i][tokorder-1] = atoi(token);
         if(tokorder!=0) cpuValues[i][tokorder-1] = atoi(token);
         token = strtok(NULL," ");
         tokorder++;
      }
   }
   
   fclose(fp); //close file
   sleep(1);
   }
}

void *Analyzer(void *thid){

   int prevIdle, idle, prevBusy, busy, prevTotal, total, totaldiff, idlediff;

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
}

void *Printer(void *thid){

   for(int i=0;i<MAXCPUS;i++) {
      //printf("Previous values CPU %d - User: %d Nice: %d System: %d Idle: %d iowait: %d irq: %d softirq: %d Steal: %d Guest: %d Guest_nice: %d \n", i, oldCpuValues[i][0], oldCpuValues[i][1],oldCpuValues[i][2],oldCpuValues[i][3],oldCpuValues[i][4],oldCpuValues[i][5],oldCpuValues[i][6],oldCpuValues[i][7],oldCpuValues[i][8],oldCpuValues[i][9]); //print previous values

      printf("Current values CPU %d - User: %d Nice: %d System: %d Idle: %d iowait: %d irq: %d softirq: %d Steal: %d Guest: %d Guest_nice: %d \n", i, cpuValues[i][0], cpuValues[i][1],cpuValues[i][2],cpuValues[i][3],cpuValues[i][4],cpuValues[i][5],cpuValues[i][6],cpuValues[i][7],cpuValues[i][8],cpuValues[i][9]); //print current values

      printf("Current CPU %d usage: %.3lf %% \n", i, percentValue[i]);
   }
}

int main(){
   pthread_create(&thread_ids[0], NULL, Reader, NULL); //run Reader
   sleep(2);
   pthread_create(&thread_ids[1], NULL, Analyzer, NULL); //run Analyzer
   pthread_create(&thread_ids[2], NULL, Printer, NULL); //run Printer

   pthread_join(thread_ids[0], NULL);
   pthread_join(thread_ids[1], NULL);
   pthread_join(thread_ids[2], NULL);
}
   
