#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main () {
    int pipe1 [2]; // Parent to child
    int pipe2 [2]; // Child to parent
    pid_t pid;
    int buffer_size = 4096;

    // TODO : Create both pipes
    // if ( pipe ( pipe1 ) == -1) { /* error handling */ }
    if ( pipe ( pipe1 ) == -1) {
        fprintf(stderr, "Error with pipe 1");
    }
    if ( pipe ( pipe2 ) == -1) {
        fprintf(stderr, "Error with pipe 2");
    }

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

        char *buffer = malloc(buffer_size);

        while (1) {
            ssize_t bytes_read = read(pipe1[0], buffer, buffer_size - 1);

            // Needed else message overflows
            buffer[bytes_read] = '\0';
            char received_message[buffer_size];
            snprintf(received_message, buffer_size, "Child received: %s", buffer);

            write(pipe2[1], received_message, strlen(received_message));
        }

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
        
        char *buffer = malloc(buffer_size);

        int bytes_read;
        while (1) {
            // Takes message from user input to send
            char message[100];
            fprintf(stderr, "Enter your message that you want to send to the child process: \n");
            scanf("%s", message);

            // Writes message to stdout
            write(pipe1[1], message, strlen(message));
            // Receives message from stdin from child process'
            ssize_t bytes_read = read(pipe2[0], buffer, buffer_size - 1);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                printf("Parent received: %s\n", buffer);
            }
        }
    }

    return 0;
}