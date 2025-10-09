# Project 1

## Build and Run Project:
1. Build each phase:
```
gcc phase1.c -pthread -o phase1.o && gcc phase2.c -pthread -o phase2.o & gcc phase3.c -pthread -o phase3.o && gcc phase4.c -pthread -o phase4.o
```
2. You can now run your chosen phase. To run your chosen phase use `./phase1.o`, where the number is the phase that you want to run:
```
./phase1.o
```

## Approaches to Each Phase

### Phase 1:
For phase 1, I decided to modify the banker example that was provided. It uses a set seed to deposit and withdraw the same amount of money with every run of the program. 
However, if you run the program, you will notice different results with each run due
to race conditions when accessing the user's account.
To consistently get race conditions to activate, I needed 500 threads and 500 operations per teller.
  
### Phase 2:
I copied my program from phase 1, and made the deposit and withdraw
logic use mutex locks to lock the account before it performs an operation
on it. This prevents the race conditions found in the earlier program.
It performs about 36% slower than phase 1 with race conditions.
  
### Phase 3:
I modified the sample deadlock scenario to fit in my program.
This version only has 2 accounts, which are both trying to transfer
money to one another at the same time. However, both threads try to lock
the account they are trying to transfer from first, causing a deadlock.
  
### Phase 4:
I used the lock ordering solution to resolve the deadlock. The program
looks at both account IDs and finds the lower one. It then attempts to
lock the lower account ID first, preventing a deadlock.