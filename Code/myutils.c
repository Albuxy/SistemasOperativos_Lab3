#include "myutils.h"

struct timeval start, end;
long mtime, seconds, useconds;    

void startTimer() {
    gettimeofday(&start, NULL);
}

long endTimer() {
    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    return mtime;
}

int readSplit( int fin, char* buff, char* s, int maxlen ) {
    int i = 0;
    int oneread = 1;
    char c = *s+1;
    while(c != *s && c != '\n' && oneread == 1 && i < maxlen) {
        oneread = read( fin, &c, 1);
        if(c != *s && c != '\n' && oneread == 1) {
            buff[i] = c;
            i++;
        }
    }
    if(c=='\n') *s = c;
    if(i < maxlen) buff[i] = '\0';
    return i;
}



