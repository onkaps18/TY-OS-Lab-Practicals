/* loader.c - Program to load ALP from file to memory */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 100
#define MEMORY_SIZE 1000

// Structure to store the program in memory
typedef struct {
    int address;
    char label[20];
    char opcode[20];
    char operand[20];
    char objectCode[20];
} Instruction;

// Function to convert hex string to integer
int hexToInt(char* hex) {
    int result = 0;
    while (*hex) {
        result = result * 16;
        if (*hex >= '0' && *hex <= '9')
            result += *hex - '0';
        else if (*hex >= 'A' && *hex <= 'F')
            result += *hex - 'A' + 10;
        else if (*hex >= 'a' && *hex <= 'f')
            result += *hex - 'a' + 10;
        hex++;
    }
    return result;
}

// Function to trim whitespace from a string
void trim(char* str) {
    char* start = str;
    char* end = str + strlen(str) - 1;
    
    while(isspace(*start)) start++;
    while(end > start && isspace(*end)) end--;
    
    *(end + 1) = '\0';
    memmove(str, start, strlen(start) + 1);
}

// Function to load program from file to memory
int loadProgram(const char* filename, Instruction* memory, int* memorySize) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    *memorySize = 0;
    int startAddress = -1;

    // Read file line by line
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        char* token;
        char lineCopy[MAX_LINE_LENGTH];
        strcpy(lineCopy, line);
        
        // Skip empty lines and comments
        if (strlen(line) <= 1 || line[0] == ';' || line[0] == '\n') 
            continue;

        // Parse the line
        token = strtok(lineCopy, " \t\n");
        if (token == NULL) continue;

        // Check if it's a label
        if (strchr(token, ':')) {
            strcpy(memory[*memorySize].label, token);
            token = strtok(NULL, " \t\n");
        } else {
            memory[*memorySize].label[0] = '\0';
        }

        // Get opcode
        if (token != NULL) {
            strcpy(memory[*memorySize].opcode, token);
            token = strtok(NULL, " \t\n");
        }

        // Get operand
        if (token != NULL) {
            strcpy(memory[*memorySize].operand, token);
            trim(memory[*memorySize].operand);
        } else {
            memory[*memorySize].operand[0] = '\0';
        }

        // Set address
        if (startAddress == -1) {
            // If first instruction, check if there's a START directive
            if (strcmp(memory[*memorySize].opcode, "START") == 0) {
                startAddress = hexToInt(memory[*memorySize].operand);
                memory[*memorySize].address = startAddress;
            } else {
                startAddress = 0;
                memory[*memorySize].address = 0;
            }
        } else {
            memory[*memorySize].address = memory[*memorySize - 1].address + 3;
        }

        (*memorySize)++;
    }

    fclose(file);
    return 1;
}

// Function to display the loaded program
// Function to display the loaded program
void displayProgram(Instruction* memory, int memorySize) {
    printf("\nLoaded Program:\n");
    printf("Address  Label\tOpcode\tOperand\n");
    printf("--------------------------------\n");
    
    for (int i = 0; i < memorySize; i++) {
        printf("%d     ", memory[i].address); // Use %d for decimal
        if (strlen(memory[i].label) > 0)
            printf("%s\t", memory[i].label);
        else
            printf("\t");
        printf("%s\t%s\n", memory[i].opcode, memory[i].operand);
    }
}


int main() {
    Instruction memory[MEMORY_SIZE];
    int memorySize;
    char filename[100];

    printf("Enter the input file name: ");
    scanf("%s", filename);

    if (loadProgram(filename, memory, &memorySize)) {
        displayProgram(memory, memorySize);
    }

    return 0;
}