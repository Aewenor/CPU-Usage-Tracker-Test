#include "globals.h"
#include "logger.h"

void *Logger(){
    while(!sgnl){
        FILE *log = fopen("log.txt","a+"); //open file
            if(logmess!=NULL){
                pthread_mutex_lock(&logmutex);
                fprintf(log, "%s \n", logmess);
                memset(logmess,0, sizeof(logmess));
                pthread_mutex_unlock(&logmutex);
            }
        fclose(log);

        sleep(2);
    }
   return EXIT_SUCCESS;
}
