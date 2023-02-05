#include "libs.h"
#include "globals.h"
#include "reader.h"

void *Reader(void *thid){

   char string[100];
   char* token;
   int tokorder;
   FILE* fp;

   pthread_mutex_lock(&mutex); //enable mutex

   memcpy(oldCpuValues,cpuValues, sizeof(cpuValues)); //save old values

   fp = fopen("/proc/stat","r"); //open file
   pthread_mutex_lock(&logmutex);
   if(fp==NULL) strcpy(logmess,"Reader: Could not open /proc/stat");
   else strcpy(logmess,"Reader: Successfully opened /proc/stat");
   pthread_mutex_unlock(&logmutex);
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

   pthread_mutex_unlock(&mutex); //release mutex

   watchTimer=0;

   return thid;
}
