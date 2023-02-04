#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_t thread_ids[5]; //IDs of threads
int cpuValues[10]; //Storage for cpu values

void *Reader(void *thid){

   FILE* fp = fopen("/proc/stat","r"); //open file

   fscanf(fp, "%*s %d %d %d %d %d %d %d %d %d %d", &cpuValues[0], &cpuValues[1],&cpuValues[2],&cpuValues[3],&cpuValues[4],&cpuValues[5],&cpuValues[6],&cpuValues[7],&cpuValues[8],&cpuValues[9]);

   fclose(fp); //close file

   printf("User: %d Nice: %d System: %d Idle: %d iowait: %d irq: %d softirq: %d Steal: %d Guest: %d Guest_nice: %d", cpuValues[0], cpuValues[1],cpuValues[2],cpuValues[3],cpuValues[4],cpuValues[5],cpuValues[6],cpuValues[7],cpuValues[8],cpuValues[9]); //print values (temporary help)

   sleep(1); //wait a second
}

int main(){
   pthread_create(&thread_ids[0], NULL, Reader, NULL);
   pthread_join(thread_ids[0], NULL);
}
