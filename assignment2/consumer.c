#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <time.h>

int main ( int argc , char *argv [] ) {
    int max_lines = -1; // -1 means unlimited
    int verbose = 0;
    int count;
    int line_count = 0;
    int char_count = 0;

    // Parse arguments ( - n max_lines , -v verbose )
    for( count = 0; count < argc; count++ ) {
        // printf("CMD arguments:  argv[%d]   %s\n", count, argv[count] );	
        if (strcmp(argv[count], "-n") == 0) {
            max_lines = atoi(argv[count + 1]);
            // printf("Max Lines: %d\n", max_lines);
        }
        if (strcmp(argv[count], "-v") == 0) {
            verbose = 1;
            // printf("Verbosity: %d\n", verbose);
        }		
    }

    // Reads from stdin line by line
    // Count lines and characters
    // If verbose , echo lines to stdout
    char *line = NULL;
    size_t len = 32;
    int read;

    while ((read = getline(&line, &len, stdin)) != -1) {
        char_count += read;
        line_count++;
        if (verbose) {
            printf("%s", line);
            if (max_lines != -1 && line_count >= max_lines)
                break;
        }
    }
    fflush(stdout);     // Necessary to flush because stdout is buffered while stderr is not causing out of order printing. 
    free(line);

    // Print statistics to stderr
    fprintf(stderr, "\nLines: %d, Characters: %d\n", line_count, char_count);

    return 0;
}