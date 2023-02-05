#include "libs.h"
#include "globals.h"
#include "logger.h"

void *Logger(void *thid){
    while(!sgnl){
        FILE *log = fopen("log.txt","a+"); //open file
            if(!strcmp(logmess,"")){
                pthread_mutex_lock(&logmutex);
                fprintf(log, "%s \n", logmess);
                memset(logmess,0, sizeof(logmess));
                pthread_mutex_unlock(&logmutex);
            }
        fclose(log);

        sleep(2);
    }
   return thid;
}
