#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "myutils.h"

int SOLUTION = 2; // Default implementation monitors, but reading from argv

pthread_mutex_t global_mutex;

typedef struct sAccount {
	int id;
	double balance;
	bool bInUse;             // the account is being used?
	pthread_mutex_t mutex;   // lock to use/modify vars
	pthread_cond_t  freeAcc; // we wait in it when in Use
} Account;

#define N 4               // Number of Accounts
Account bank[N];

int NThreads = 400;       // Number of Threads


void do_something() {
	int n = 20;
	int sum = 0;
	for(int i=0;i<n;i++) 
	for(int j=0;j<n;j++) 
	for(int k=0;k<n;k++) sum++;
}

bool withdraw(Account* acc, double amount) {
	if(acc->balance - amount >= 0)  { 
		acc->balance -= amount; 
		do_something();
		return true;
	}
	return false;
}
void deposit(Account* acc, double amount) {
	acc->balance += amount;
	do_something();
}



void lock_monitors(Account* acc) {
	pthread_mutex_lock(&acc->mutex);
	while(acc->bInUse){
		pthread_cond_wait(&acc->freeAcc,&acc->mutex);
	}
	acc->bInUse = true;
	pthread_mutex_unlock(&acc->mutex);
}

void unlock_monitors(Account* acc) {
	pthread_mutex_lock(&acc->mutex);
	acc->bInUse = false;
	pthread_cond_signal(&acc->freeAcc);
	pthread_mutex_unlock(&acc->mutex);
}

void transfer(Account* from, Account* to, double amount) {
	

	// ******************************************
	// transfer operation
	
	switch (SOLUTION)
	{
		case 1: pthread_mutex_lock(&global_mutex);
			break;
		case 2: if (from->id < to->id){
				pthread_mutex_lock(&from->mutex);
				pthread_mutex_lock(&to->mutex);
			} else {
				pthread_mutex_lock(&to->mutex);
				pthread_mutex_lock(&from->mutex);
			}
			break;
		case 3: if (from->id < to->id){
				lock_monitors(from);
				lock_monitors(to);
			} else {
				lock_monitors(to);
				lock_monitors(from);
			}
			break;
		case 4: sem_wait(&sem_global);
			break;
		default: 
			printf ("ERROR");
	} 		

	bool bDone = withdraw(from,amount);
	if(bDone) deposit(to,amount);
	// end of transfer operation 
	// ******************************************
			
	switch (SOLUTION)
	{
		case 1: pthread_mutex_unlock(&global_mutex);
			break;
		case 2: if (from->id < to->id){
				pthread_mutex_unlock(&from->mutex);
				pthread_mutex_unlock(&to->mutex);
			} else {
				pthread_mutex_unlock(&to->mutex);
				pthread_mutex_unlock(&from->mutex);
			}
			break;
		case 3: if (from->id < to->id){
				unlock_monitors(from);
				unlock_monitors(to);
			} else {
				unlock_monitors(to);
				unlock_monitors(from);
			}
			break;
		case 4: sem_signal(&sem_global);
			break;
		default: 
			printf ("ERROR");
	}
}


double bankInit() {
	double sum = 0;
	for(int i=0;i<N;i++) {
		bank[i].id = i;
		bank[i].balance = 100;
		bank[i].bInUse = false;
		sum += bank[i].balance;

		// Initialize lock and condition
	}
	sem_init(&sem_global,2);
	return sum;
}


void* tranferThread( void* param ) {
	int from = -1;
	int to = -1;
	int iter = 1000;
	while(iter--) {
		from = -1;
		to = -1;
		while(from == to) {
			from = rand()%N;
			to = rand()%N;
		}
		transfer(&bank[from], &bank[to], rand()%50+1);
	}
	return NULL;
}



int main(int argc, char *argv[])
{
	if(argc > 1) SOLUTION = atoi(argv[1]);
	if(argc > 2) NThreads = atoi(argv[2]);

	double sum = bankInit();
    printf("Initial bank capital: %f\n",sum);

	pthread_t* tids;          					/* the thread identifiers */
	tids = malloc(NThreads * sizeof(pthread_t));

	
    printf("Creating %d Threads\n",NThreads);
	
	startTimer();
    for(int i=0;i<NThreads;i++) {
        pthread_create(&tids[i], NULL, tranferThread, NULL);
    }

    printf("Main : waiting for threads...\n");
    for(int i=0;i<NThreads;i++) pthread_join(tids[i], NULL);

    free(tids);

    double sumEnd = 0;
	for(int i=0;i<N;i++) {
		printf("Account %d balance : %f\n", i, bank[i].balance);
		sumEnd += bank[i].balance;
	}
    
    if(sumEnd != sum) printf("ERROR : ******** CORRUPT BANK!!!!!!  sum: %lf *******\n",sumEnd);
    else printf("Final bank sum: %f\n",sum);
	
    printf("Done: %ld millis\n",endTimer());


	return 0;
}
