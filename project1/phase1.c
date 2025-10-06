#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_ACCOUNTS 5
#define NUM_THREADS 500
#define TRANSACTIONS_PER_TELLER 500
#define INITIAL_BALANCE 1000.0

// Shared data structure
typedef struct {
    int account_id;
    double balance;
    int transaction_count;
} Account;

// Global accounts array (shared resource)
Account accounts[NUM_ACCOUNTS];

// Thread function
void* teller_thread(void* arg) {
    int teller_id = *(int*)arg;

    double deposit_amount = 100;
    double withdraw_amount = 50;

    // Random set seed to get reproductible runs
    unsigned int seed = 235;
    
    // Perform multiple transactions
    for (int i = 0; i < TRANSACTIONS_PER_TELLER; i++) {
        // Select random account
        int selected_account = rand_r(&seed) % NUM_ACCOUNTS;

        // Perform deposit or withdrawal
        // THIS WILL HAVE RACE CONDITIONS!
        if (rand_r(&seed) % 2 == 0) {
            // printf("Thread %d: Depositing %.2f\n", teller_id, deposit_amount);
            accounts[selected_account].balance += deposit_amount;
        } 
        else {
             // printf("Thread %d: Withdrawing %.2f\n", teller_id, withdraw_amount);
             accounts[selected_account].balance -= withdraw_amount;
         }

        accounts[selected_account].transaction_count++;
        
        // printf("Teller %d: Transaction %d\n", teller_id, i);
    }
    
    return NULL;
}

int main() {
    for (int i = 0; i < NUM_ACCOUNTS; i++) {
        accounts[i].account_id = i;
        accounts[i].balance = INITIAL_BALANCE;
        accounts[i].transaction_count = 0;
    }

    printf("Initial balance of account 0: %.2f\n", accounts[0].balance);

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

    printf("Final balance: %.2f\n", accounts[0].balance);

    return 0;
}