#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_t thread_ids[5]; //IDs of threads
int cpuValues[10]; //Storage for current cpu values
int oldCpuValues[10]; //Storage for previous cpu values
double percentValue; //percentage usage of cpu

void *Reader(void *thid){

   FILE* fp = fopen("/proc/stat","r"); //open file

   fscanf(fp, "%*s %d %d %d %d %d %d %d %d %d %d", &oldCpuValues[0], &oldCpuValues[1],&oldCpuValues[2],&oldCpuValues[3],&oldCpuValues[4],&oldCpuValues[5],&oldCpuValues[6],&oldCpuValues[7],&oldCpuValues[8],&oldCpuValues[9]); //save old values

   sleep(1); //wait a second
   rewind(fp); //back to the beginning of the file

   fscanf(fp, "%*s %d %d %d %d %d %d %d %d %d %d", &cpuValues[0], &cpuValues[1],&cpuValues[2],&cpuValues[3],&cpuValues[4],&cpuValues[5],&cpuValues[6],&cpuValues[7],&cpuValues[8],&cpuValues[9]); //save current values

   fclose(fp); //close file
}

void *Analyzer(void *thid){
	int prevIdle = oldCpuValues[3]+oldCpuValues[4]; //previous idle + iowait
	int idle = cpuValues[3]+cpuValues[4]; //current idle + iowait

	int prevBusy = oldCpuValues[0]+oldCpuValues[1]+oldCpuValues[2]+oldCpuValues[5]+oldCpuValues[6]+oldCpuValues[7]; //previous user + nice + system + irq + softirq + steal

	int busy = cpuValues[0]+cpuValues[1]+cpuValues[2]+cpuValues[5]+cpuValues[6]+cpuValues[7]; //current user + nice + system + irq + softirq + steal

	int prevTotal = prevBusy+prevIdle;
	int total = busy+idle;

	int totaldiff = total - prevTotal;
	int idlediff = idle - prevIdle;

	percentValue = ((double)(totaldiff - idlediff)/totaldiff)*100.0;
}

int main(){
   pthread_create(&thread_ids[0], NULL, Reader, NULL);
   sleep(3);
   printf("User: %d Nice: %d System: %d Idle: %d iowait: %d irq: %d softirq: %d Steal: %d Guest: %d Guest_nice: %d \n", cpuValues[0], cpuValues[1],cpuValues[2],cpuValues[3],cpuValues[4],cpuValues[5],cpuValues[6],cpuValues[7],cpuValues[8],cpuValues[9]); //print values (temporary help)
   printf("User: %d Nice: %d System: %d Idle: %d iowait: %d irq: %d softirq: %d Steal: %d Guest: %d Guest_nice: %d \n", oldCpuValues[0], oldCpuValues[1],oldCpuValues[2],oldCpuValues[3],oldCpuValues[4],oldCpuValues[5],oldCpuValues[6],oldCpuValues[7],oldCpuValues[8],oldCpuValues[9]); //print values (temporary help)
   pthread_create(&thread_ids[0], NULL, Reader, NULL);
   sleep(3);
   printf("User: %d Nice: %d System: %d Idle: %d iowait: %d irq: %d softirq: %d Steal: %d Guest: %d Guest_nice: %d \n", cpuValues[0], cpuValues[1],cpuValues[2],cpuValues[3],cpuValues[4],cpuValues[5],cpuValues[6],cpuValues[7],cpuValues[8],cpuValues[9]); //print values (temporary help)
   printf("User: %d Nice: %d System: %d Idle: %d iowait: %d irq: %d softirq: %d Steal: %d Guest: %d Guest_nice: %d \n", oldCpuValues[0], oldCpuValues[1],oldCpuValues[2],oldCpuValues[3],oldCpuValues[4],oldCpuValues[5],oldCpuValues[6],oldCpuValues[7],oldCpuValues[8],oldCpuValues[9]); //print values (temporary help)
   pthread_create(&thread_ids[1], NULL, Analyzer, NULL);
   sleep(1);
   printf("Percentage usage: %lf %%", percentValue); //print values (temporary help)

   pthread_join(thread_ids[0], NULL);
   pthread_join(thread_ids[1], NULL);
}
   
