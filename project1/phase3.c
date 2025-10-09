#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define NUM_ACCOUNTS 2
#define NUM_THREADS 2
#define INITIAL_BALANCE 1000.0
#define TRANSACTION_AMOUNT 1

// Shared data structure
typedef struct {
    int account_id;
    double balance;
    int transaction_count;
    pthread_mutex_t lock;
} Account;

// Global accounts array (shared resource)
Account accounts[NUM_ACCOUNTS];

void transfer(int from_id, int to_id, double amount) {
    printf("Thread %ld: Attempting transfer from %d to %d\n",
    pthread_self(), from_id, to_id);

    pthread_mutex_lock(&accounts[from_id].lock) ;
    printf("Thread %ld: Locked account %d\n", pthread_self(), from_id);

    // Simulate processing delay - gives other thread time to create
    usleep(100);

    printf("Thread %ld: Waiting for account %d\n", pthread_self(), to_id);
    time_t start = time ( NULL ) ;
    while (pthread_mutex_trylock(&accounts[to_id].lock) != 0) {
        if (time(NULL) - start > 5) {
            printf("Possible deadlock detected !\n");
            break;
        }
   }

    // If we get here, no deadlock occurred this time
    accounts[from_id].balance -= amount;
    accounts[to_id].balance += amount;

    pthread_mutex_unlock(&accounts[to_id].lock);
    pthread_mutex_unlock(&accounts[from_id].lock);
}

// Thread function
void* teller_thread(void* arg) {
    int teller_id = *(int*)arg;

    for (int i = 0; i < TRANSACTION_AMOUNT; i++) {
        if (teller_id == 0) {
            transfer(0, 1, 50.0);
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