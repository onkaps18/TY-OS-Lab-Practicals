#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 100
#define MAX_RESOURCES 100

// Function to check if the system is in a safe state (same as before)
bool isSafeState(int processes, int resources, 
                 int available[], 
                 int maximum[][MAX_RESOURCES],
                 int allocation[][MAX_RESOURCES],
                 int need[][MAX_RESOURCES],
                 int safeSequence[]) {
    
    bool finish[MAX_PROCESSES] = {false};
    int work[MAX_RESOURCES];
    
    // Initialize work = available
    for(int i = 0; i < resources; i++)
        work[i] = available[i];
    
    int count = 0;
    
    while(count < processes) {
        bool found = false;
        
        for(int p = 0; p < processes; p++) {
            if(finish[p] == false) {
                bool possible = true;
                
                for(int r = 0; r < resources; r++) {
                    if(need[p][r] > work[r]) {
                        possible = false;
                        break;
                    }
                }
                
                if(possible) {
                    for(int r = 0; r < resources; r++)
                        work[r] += allocation[p][r];
                    
                    safeSequence[count] = p;
                    finish[p] = true;
                    count++;
                    found = true;
                }
            }
        }
        
        if(found == false)
            return false;
    }
    
    return true;
}

int main() {
    int processes, resources;
    int available[MAX_RESOURCES];
    int maximum[MAX_PROCESSES][MAX_RESOURCES];
    int allocation[MAX_PROCESSES][MAX_RESOURCES];
    int need[MAX_PROCESSES][MAX_RESOURCES];
    int safeSequence[MAX_PROCESSES];
    
    // Input number of processes and resources
    printf("Enter number of processes: ");
    scanf("%d", &processes);
    printf("Enter number of resources: ");
    scanf("%d", &resources);
    
    // Input available resources
    printf("\nEnter available resources:\n");
    for(int i = 0; i < resources; i++) {
        printf("Resource %d: ", i);
        scanf("%d", &available[i]);
    }
    
    // Input maximum resources for each process
    printf("\nEnter maximum resources for each process:\n");
    for(int i = 0; i < processes; i++) {
        printf("\nFor Process %d:\n", i);
        for(int j = 0; j < resources; j++) {
            printf("Resource %d: ", j);
            scanf("%d", &maximum[i][j]);
        }
    }
    
    // Input allocated resources for each process
    printf("\nEnter allocated resources for each process:\n");
    for(int i = 0; i < processes; i++) {
        printf("\nFor Process %d:\n", i);
        for(int j = 0; j < resources; j++) {
            printf("Resource %d: ", j);
            scanf("%d", &allocation[i][j]);
            
            // Validate allocation against maximum
            if(allocation[i][j] > maximum[i][j]) {
                printf("\nError: Allocation cannot exceed maximum claim!\n");
                return 1;
            }
        }
    }
    
    // Calculate need matrix
    for(int i = 0; i < processes; i++) {
        for(int j = 0; j < resources; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
    
    // Print current state
    printf("\nSystem State:\n");
    printf("\nAvailable Resources: ");
    for(int i = 0; i < resources; i++)
        printf("%d ", available[i]);
        
    printf("\n\nAllocation Matrix:\n");
    for(int i = 0; i < processes; i++) {
        printf("P%d: ", i);
        for(int j = 0; j < resources; j++)
            printf("%d ", allocation[i][j]);
        printf("\n");
    }
    
    printf("\nMaximum Matrix:\n");
    for(int i = 0; i < processes; i++) {
        printf("P%d: ", i);
        for(int j = 0; j < resources; j++)
            printf("%d ", maximum[i][j]);
        printf("\n");
    }
    
    printf("\nNeed Matrix:\n");
    for(int i = 0; i < processes; i++) {
        printf("P%d: ", i);
        for(int j = 0; j < resources; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }
    
    // Check if system is in safe state
    if(isSafeState(processes, resources, available, maximum, allocation, need, safeSequence)) {
        printf("\nSystem is in SAFE state.");
        printf("\nSafe sequence is: ");
        for(int i = 0; i < processes; i++)
            printf("P%d ", safeSequence[i]);
        printf("\n");
    } else {
        printf("\nSystem is in UNSAFE state.\n");
    }
    
    return 0;
}


// Enter number of processes: 5
// Enter number of resources: 3

// Enter available resources:
// Resource 0: 3
// Resource 1: 3
// Resource 2: 2

// Enter maximum resources for each process:

// For Process 0:
// Resource 0: 7
// Resource 1: 5
// Resource 2: 3

// For Process 1:
// Resource 0: 3
// Resource 1: 2
// Resource 2: 2

// For Process 2:
// Resource 0: 9
// Resource 1: 0
// Resource 2: 2

// For Process 3:
// Resource 0: 2
// Resource 1: 2
// Resource 2: 2

// For Process 4:
// Resource 0: 4
// Resource 1: 3
// Resource 2: 3

// Enter allocated resources for each process:

// For Process 0:
// Resource 0: 0
// Resource 1: 1
// Resource 2: 0

// For Process 1:
// Resource 0: 2
// Resource 1: 0
// Resource 2: 0

// For Process 2:
// Resource 0: 3
// Resource 1: 0
// Resource 2: 2

// For Process 3:
// Resource 0: 2
// Resource 1: 1
// Resource 2: 1

// For Process 4:
// Resource 0: 0
// Resource 1: 0
// Resource 2: 2

// System State:

// Available Resources: 3 3 2 

// Allocation Matrix:
// P0: 0 1 0 
// P1: 2 0 0 
// P2: 3 0 2 
// P3: 2 1 1 
// P4: 0 0 2 

// Maximum Matrix:
// P0: 7 5 3 
// P1: 3 2 2 
// P2: 9 0 2 
// P3: 2 2 2 
// P4: 4 3 3 

// Need Matrix:
// P0: 7 4 3 
// P1: 1 2 2 
// P2: 6 0 0 
// P3: 0 1 1 
// P4: 4 3 1 