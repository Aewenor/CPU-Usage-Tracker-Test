#include "main.h"
#include "globals.h"
#include "reader.h"
#include "analyzer.h"
#include "printer.h"
#include "logger.h"
#include "watchdog.h"

volatile sig_atomic_t sgnl = 0; //for SIGTERM handling

pthread_t thread_ids[5]; //IDs of threads
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //initialize mutex
pthread_mutex_t logmutex = PTHREAD_MUTEX_INITIALIZER; //initialize mutex for logger

int cpuValues[MAXCPUS][10]; //Storage for current cpu values
int oldCpuValues[MAXCPUS][10]; //Storage for previous cpu values
double percentValue[MAXCPUS]; //percentage usage of cpu

char logmess[200]; 

int watchTimer=0;

void sigRec(int term){ 
   printf("SIGTERM signal %d received. Cleaning memory. \n", term);
   sgnl=1; //set SIGTERM flag to 1
}

int main(){

   fclose(fopen("log.txt","w")); //clear log file

   struct sigaction action; //signal action init
   memset(&action,0,sizeof(struct sigaction)); //set memory
   action.sa_handler = sigRec; //set handler to sigRec function
   sigaction(SIGTERM, &action, NULL); //set SIGTERM to be handled by &action

pthread_create(&thread_ids[4], NULL, Watchdog, NULL); //create Watchdog
pthread_detach(thread_ids[4]); //run Watchdog
pthread_create(&thread_ids[3], NULL, Logger, NULL); //create Logger
pthread_detach(thread_ids[3]); //run Logger

   while(!sgnl){
      pthread_create(&thread_ids[0], NULL, Reader, NULL); //create Reader
      pthread_create(&thread_ids[1], NULL, Analyzer, NULL); //create Analyzer
      pthread_create(&thread_ids[2], NULL, Printer, NULL); //create Printer

      pthread_join(thread_ids[0], NULL); //run Reader
      sleep(1);
      pthread_join(thread_ids[1], NULL); //run Analyzer
      pthread_join(thread_ids[2], NULL); //run Printer
   }

   sleep(3);
   pthread_mutex_destroy(&mutex); //free mutex
   pthread_mutex_destroy(&logmutex); //free logger mutex
   
   pthread_exit(NULL);
}
 
