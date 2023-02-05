#include "libs.h"
#include "globals.h"
#include "watchdog.h"

void *Watchdog(void *thid){

       while(!sgnl){
        if(watchTimer>10) {
            printf("Time exceeded, sending SIGTERM. \n");
            strcpy(logmess,"Watchdog: Timer out of boundaries.");
            sgnl=1;
        }
        sleep(1);
        watchTimer++;
    }

   return thid;
}
