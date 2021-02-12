#include "mysemaphore.h"


void sem_init(semaphore* sem, int i) {
	sem->i = i;
	pthread_mutex_init(&sem->lock, NULL);
    pthread_cond_init(&sem->cond, NULL);
}


void sem_wait(semaphore* sem) {
	 pthread_mutex_lock(&sem->lock);
	 while(sem->i == 0) {
	 	pthread_cond_wait(&sem->cond, &sem->lock);
	 }
	 sem->i--;
	 pthread_mutex_unlock(&sem->lock);
}


void sem_signal(semaphore* sem) {
	 pthread_mutex_lock(&sem->lock);
	 sem->i++;
	 pthread_cond_signal(&sem->cond);
	 pthread_mutex_unlock(&sem->lock);
}
