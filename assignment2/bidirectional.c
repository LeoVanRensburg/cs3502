#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main () {
    int pipe1 [2]; // Parent to child
    int pipe2 [2]; // Child to parent
    pid_t pid;

    // TODO : Create both pipes
    // if ( pipe ( pipe1 ) == -1) { /* error handling */ }
    if ( pipe ( pipe1 ) == -1) {
        fprintf(stderr, "Error with pipe 1");
    }
    if ( pipe ( pipe2 ) == -1) {
        fprintf(stderr, "Error with pipe 2");
    }

    // Allocates buffer before forking
    int buffer_size = 4096;
    char *buffer = malloc(buffer_size);

    // TODO : Fork process
    if((pid = fork()) == -1)
                {
                    fprintf(stderr, "fork failed for some reason");
                    exit(1);
                }

    if ( pid == 0) {
        // Child process
        // TODO : Close unused pipe ends
        // close ( pipe1 [1]) ; // Close write end of pipe1
        // close ( pipe2 [0]) ; // Close read end of pipe2
        // TODO : Read from parent , send responses
        fprintf(stderr, "Hello world1!, process_id(pid) = %d \n",getpid());
        close (pipe1[1]);
        close (pipe2[0]);
    } else {
        // Parent process
        // TODO : Close unused pipe ends
        // close ( pipe1 [0]) ; // Close read end of pipe1
        // close ( pipe2 [1]) ; // Close write end of pipe2
        // TODO : Send messages , read responses
        // TODO : wait () for child
        fprintf(stderr, "Hello world!, process_id(pid) = %d \n",getpid());
        close (pipe1[0]);
        close (pipe2[1]);
    }

    return 0;
}