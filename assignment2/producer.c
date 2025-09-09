#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

int main ( int argc , char *argv []) {
    FILE *input = stdin ;
    int buffer_size = 4096;
    char opt;
    int count;
    char filename[] = "test.txt";

    // TODO : Parse command line arguments
    // -f filename ( optional )
    // -b buffer_size ( optional )
    for( count = 0; count < argc; count++ ) {
	// printf("CMD arguments:  argv[%d]   %s\n", count, argv[count] );	
	if (strcmp(argv[count], "-f") == 0) {
		strcpy(filename, argv[count + 1]);
		printf("File name: %s\n", filename);
	}
	if (strcmp(argv[count], "-b") == 0) {
		buffer_size = atoi(argv[count + 1]);
		printf("Buffer size: %d\n", buffer_size);
	}
			
    }
    // TODO : Open file if -f provided

    // TODO : Allocate buffer

    // TODO : Read from input and write to stdout

    // TODO : Cleanup
    return 0;
}
