#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(){
    FILE* files = fopen("/proc/stat","r");
    fclose(files);
    assert(files!=NULL);
    printf("/proc/stat successfully opened. \n");
    return 0;
}
