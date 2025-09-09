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
    int open_file = 0;

    // TODO : Parse command line arguments
    // -f filename ( optional )
    // -b buffer_size ( optional )
    // Add error checking potentially?
    for( count = 0; count < argc; count++ ) {
	// printf("CMD arguments:  argv[%d]   %s\n", count, argv[count] );	
	if (strcmp(argv[count], "-f") == 0) {
		strcpy(filename, argv[count + 1]);
		open_file = 1;
		printf("File name: %s\n", filename);
	}
	if (strcmp(argv[count], "-b") == 0) {
		buffer_size = atoi(argv[count + 1]);
		printf("Buffer size: %d\n", buffer_size);
	}
			
    }

    // TODO : Open file if -f provided
    char file_info[5000];
    if (open_file) {
	FILE* fptr;
	fptr = fopen(filename, "r");
	if (fptr == NULL) {
		printf("The file doesn't exist or something");
	} else {
		while (fgets(file_info, 500, fptr) != NULL) {
			printf("%s", file_info);
		}
	fclose(fptr);
	}
    }
	

    // TODO : Allocate buffer

    // TODO : Read from input and write to stdout

    // TODO : Cleanup
    return 0;
}
