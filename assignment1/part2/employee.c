#include <stdio.h>
#include <string.h>

int main() {
    // Character array to store name
    // In C, strings are arrays of characters ending with '\0'
    char name[50];
    int employee_id;
    float hours_worked;

    printf("=============================\n");

    printf("Enter your name: ");
    // fgets reads a line including spaces
    // stdin means "standard input" (keyboard)
    fgets(name, sizeof(name), stdin);

    // Remove newline that fgets includes
    name[strcspn(name, "\n")] = '\0';

    printf("Enter your employee ID: ");
    // & means "address of"
    scanf("%d", &employee_id);

    printf("Hours worked this week: ");
    scanf("%f", &hours_worked);

    printf("\nEmployee Summary:\n");
    printf("Name: %s\n", name); // %s for string
    printf("ID: %d\n", employee_id); // %d for integer
    printf("Hours: %.2f\n", hours_worked); // %.2f for float with 2 decimals

    return 0;
}
