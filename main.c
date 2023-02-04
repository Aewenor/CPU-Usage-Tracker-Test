#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>

#define MAXCPUS 6 //number of available CPU cores

volatile sig_atomic_t sgnl = 0; //for SIGTERM handling

pthread_t thread_ids[5]; //IDs of threads
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //initialize mutex

int cpuValues[MAXCPUS][10]; //Storage for current cpu values
int oldCpuValues[MAXCPUS][10]; //Storage for previous cpu values
double percentValue[MAXCPUS]; //percentage usage of cpu

void sigRec(){ 
   printf("SIGTERM signal received \n");
   sgnl=1; //set SIGTERM flag to 1
}

void *Reader(){

   char string[100];
   char* token;
   int tokorder;
   FILE* fp;

   pthread_mutex_lock(&mutex); //enable mutex

   memcpy(oldCpuValues,cpuValues, sizeof(cpuValues)); //save old values

   fp = fopen("/proc/stat","r"); //open file
   fgets(string,100,fp); //skip first line with summed cpu usage
   for(int i=0;i<MAXCPUS;i++) {
      fgets(string,100,fp); //get line from file
      token = strtok(string," "); //separate line into values
      tokorder=0; //for iteration of values
      while(token!=NULL) {
         if(tokorder!=0) cpuValues[i][tokorder-1] = atoi(token); //save value as int
         token = strtok(NULL," "); //next value
         tokorder++; //increment
      }
   }
   
   fclose(fp); //close file
   sleep(1); //wait a second

   pthread_mutex_unlock(&mutex); //release mutex

   return EXIT_SUCCESS;
}

void *Analyzer(){

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

   return EXIT_SUCCESS;
}

void *Printer(){

   pthread_mutex_lock(&mutex); //enable mutex

   for(int i=0;i<MAXCPUS;i++) {
      //printf("Previous values CPU %d - User: %d Nice: %d System: %d Idle: %d iowait: %d irq: %d softirq: %d Steal: %d Guest: %d Guest_nice: %d \n", i, oldCpuValues[i][0], oldCpuValues[i][1],oldCpuValues[i][2],oldCpuValues[i][3],oldCpuValues[i][4],oldCpuValues[i][5],oldCpuValues[i][6],oldCpuValues[i][7],oldCpuValues[i][8],oldCpuValues[i][9]); //print previous values

      //printf("Current values CPU %d - User: %d Nice: %d System: %d Idle: %d iowait: %d irq: %d softirq: %d Steal: %d Guest: %d Guest_nice: %d \n", i, cpuValues[i][0], cpuValues[i][1],cpuValues[i][2],cpuValues[i][3],cpuValues[i][4],cpuValues[i][5],cpuValues[i][6],cpuValues[i][7],cpuValues[i][8],cpuValues[i][9]); //print current values

      printf("Current CPU %d usage: %.3lf %% \n", i, percentValue[i]); //print percentage values for CPU cores
   }

   pthread_mutex_unlock(&mutex); //release mutex

   return EXIT_SUCCESS;
}

int main(){

   struct sigaction action; //signal action init
   memset(&action,0,sizeof(struct sigaction)); //set memory
   action.sa_handler = sigRec; //set handler to sigRec function
   sigaction(SIGTERM, &action, NULL); //set SIGTERM to be handled by &action

   while(!sgnl){
      pthread_create(&thread_ids[0], NULL, Reader, NULL); //create Reader
      pthread_create(&thread_ids[1], NULL, Analyzer, NULL); //create Analyzer
      pthread_create(&thread_ids[2], NULL, Printer, NULL); //create Printer

      pthread_join(thread_ids[0], NULL); //run Reader
      pthread_join(thread_ids[1], NULL); //run Analyzer
      pthread_join(thread_ids[2], NULL); //run Printer
   }

   pthread_mutex_destroy(&mutex); //free mutex
   
}
 
