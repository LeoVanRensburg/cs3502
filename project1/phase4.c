#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define NUM_ACCOUNTS 2
#define NUM_THREADS 2
#define INITIAL_BALANCE 1000.0
#define TRANSACTION_AMOUNT 10000

// Shared data structure
typedef struct {
    int account_id;
    double balance;
    int transaction_count;
    pthread_mutex_t lock;
} Account;

// Global accounts array (shared resource)
Account accounts[NUM_ACCOUNTS];

void transfer (int from_id, int to_id, double amount) {
    printf("Thread %ld: Attempting transfer from %d to %d\n", pthread_self(), from_id, to_id);

    // Always lock lower ID first
    int first = (from_id < to_id) ? from_id : to_id;
    int second = (from_id < to_id) ? to_id : from_id;

    pthread_mutex_lock(&accounts[first].lock);
    printf("Thread %ld: Locked account %d\n", pthread_self(), first);
    pthread_mutex_lock(&accounts[second].lock);
    printf("Thread %ld: Locked account %d\n", pthread_self(), second);

    // Perform transfer
    accounts[from_id].balance -= amount;
    accounts[to_id].balance += amount;

    pthread_mutex_unlock(&accounts[second].lock);
    pthread_mutex_unlock(&accounts[first].lock);
}

// Thread function
void* teller_thread(void* arg) {
    int teller_id = *(int*)arg;

    for (int i = 0; i < TRANSACTION_AMOUNT; i++) {
        if (teller_id == 0) {
            transfer(0, 1, 100.0);
        } else {
            transfer(1, 0, 50.0);
        }
    }
    
    return NULL;
}

int main() {
    for (int i = 0; i < NUM_ACCOUNTS; i++) {
        pthread_mutex_init(&accounts[i].lock, NULL);
        accounts[i].account_id = i;
        accounts[i].balance = INITIAL_BALANCE;
        accounts[i].transaction_count = 0;
    }

    for (int i = 0; i < NUM_ACCOUNTS; i++) {
        printf("Initial balance of account %d: %.2f\n", i, accounts[i].balance);
    }

    // Creating threads (see Appendix \ref{sec:voidpointer} for void * explanation)
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, teller_thread, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < NUM_ACCOUNTS; i++) {
        printf("Final balance of account %d: %.2f\n", i, accounts[i].balance);
        pthread_mutex_destroy(&accounts[i].lock);
    }

    return 0;
}