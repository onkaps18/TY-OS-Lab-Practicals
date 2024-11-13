#include <stdio.h>
#include <stdlib.h>
#define MAX_LINE_LENGTH 1024

// Function to copy file character by character
void copyFileByChar(const char* source, const char* destination) {
    FILE *sourceFile, *destFile;
    int ch;

    // Open source file in read mode
    sourceFile = fopen(source, "r");
    if (sourceFile == NULL) {
        printf("Error opening source file!\n");
        return;
    }

    // Open destination file in write mode
    destFile = fopen(destination, "w");
    if (destFile == NULL) {
        printf("Error opening destination file!\n");
        fclose(sourceFile);
        return;
    }

    // Read and write character by character
    while ((ch = fgetc(sourceFile)) != EOF) {
        fputc(ch, destFile);
    }

    printf("File copied successfully character by character!\n");

    // Close both files
    fclose(sourceFile);
    fclose(destFile);
}

// Function to copy file line by line
void copyFileByLine(const char* source, const char* destination) {
    FILE *sourceFile, *destFile;
    char line[MAX_LINE_LENGTH];

    // Open source file in read mode
    sourceFile = fopen(source, "r");
    if (sourceFile == NULL) {
        printf("Error opening source file!\n");
        return;
    }

    // Open destination file in write mode
    destFile = fopen(destination, "w");
    if (destFile == NULL) {
        printf("Error opening destination file!\n");
        fclose(sourceFile);
        return;
    }

    // Read and write line by line
    while (fgets(line, MAX_LINE_LENGTH, sourceFile) != NULL) {
        fputs(line, destFile);
    }

    printf("File copied successfully line by line!\n");

    // Close both files
    fclose(sourceFile);
    fclose(destFile);
}

int main() {
    char source[100], destination[100];
    int choice;

    printf("Enter source filename: ");
    scanf("%s", source);
    
    printf("Enter destination filename: ");
    scanf("%s", destination);
    
    printf("\nChoose copying method:\n");
    printf("1. Character by character\n");
    printf("2. Line by line\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            copyFileByChar(source, destination);
            break;
        case 2:
            copyFileByLine(source, destination);
            break;
        default:
            printf("Invalid choice!\n");
    }

    return 0;
}