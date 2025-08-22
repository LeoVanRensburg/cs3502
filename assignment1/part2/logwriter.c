#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE *logfile; // Pointer to file structure
char message[100];

int main() {
    time_t current_time;

    // Open file for appending ("a" mode)
    logfile = fopen("owltech.log", "a");
    // Always check if file opened successfully
    if (logfile == NULL) {
        printf("Error: Could not open log file\n");
        return 1; // Return non-zero to indicate error
    }

    printf("Enter log message: ");
    fgets(message, sizeof(message), stdin);

    // Get current time
    time(&current_time);

    // Write to file with timestamp
    fprintf(logfile, "[%s] %s", ctime(&current_time), message);

    // Always close files when done
    fclose(logfile);
    printf("Log entry saved successfully!\n");

    // Display the log file contents
    printf("\n--- Current Log Contents ---\n");
    system("cat owltech.log");

    return 0;
}
