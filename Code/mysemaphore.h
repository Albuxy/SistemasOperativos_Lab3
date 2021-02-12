#ifndef MYSEMAPHORE
#define MYSEMAPHORE

#include <unistd.h>     // unix-like system calls read and write
#include <fcntl.h>      // unix-like file handling : open
#include <stdio.h>      // standard C lib input output basic functions compatible with Windows
#include <string.h>     // also from standard C lib : basic string functions like strlen
#include <pthread.h>    //

typedef struct semaphore_struct {
	int i;
	pthread_mutex_t lock;
	pthread_cond_t cond;
} semaphore;

void sem_init(semaphore* sem, int i);
void sem_wait(semaphore* sem);
void sem_signal(semaphore* sem);


#endif