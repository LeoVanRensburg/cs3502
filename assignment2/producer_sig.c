#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>

volatile sig_atomic_t shutdown_flag = 0;
volatile sig_atomic_t stats_flag = 0;

void handle_sigint (int sig ) {
    shutdown_flag = 1;
}

void handle_sigusr1 (int sig ) {
    stats_flag = 1;
}

int main ( int argc , char *argv []) {
    FILE *input = stdin ;
    int buffer_size = 4096;
    char opt;
    int count;
    char filename[100] = "test.txt";
    int open_file = 0;

    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // Parse command line arguments
    // -f filename ( optional )
    // -b buffer_size ( optional )
    // Add error checking potentially?
    for( count = 0; count < argc; count++ ) {
	    // printf("CMD arguments:  argv[%d]   %s\n", count, argv[count] );	
	    if (strcmp(argv[count], "-f") == 0) {
		    strcpy(filename, argv[count + 1]);
		    open_file = 1;
		    // printf("File name: %s\n", filename);     // Writes to stdout which messes up consumer counting  
	    }
	    if (strcmp(argv[count], "-b") == 0) {
		    buffer_size = atoi(argv[count + 1]);
		    // printf("Buffer size: %d\n", buffer_size);        // Writes to stdout which messer up consumer counting
	    }		
    }

    // Open file if -f provided
    if (open_file) {
	    input = fopen(filename, "r");
	    if (input == NULL) {
		    // printf("The file doesn't exist or something");       // Writes to stdout which messer up consumer counting
            return 1;
	    }
    }

    // Allocate buffer
    char *buffer = malloc(buffer_size);


    // Read from input and write to stdout
    int bytes_read;
    while (((bytes_read = fread(buffer, 1, buffer_size, input)) > 0) && shutdown_flag == stats_flag) {
        fwrite(buffer, 1, bytes_read, stdout);
    }

    // Cleanup
    free(buffer);
    if (input != stdin) {   // Makes sure that it only closes if file and not if no file was specified
        fclose(input);
    }
    return 0;
}