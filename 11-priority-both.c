#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 100

// Structure to represent a process
struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;
    int remainingTime;
    int finishTime;
    int turnaroundTime;
    int waitingTime;
};

// Function to display results
void displayResults(struct Process processes[], int n) {
    printf("\nProcess ID | Finish Time | Turnaround Time | Waiting Time\n");
    for (int i = 0; i < n; i++) {
        printf("    %d       |      %d       |       %d       |     %d\n",
               processes[i].id, processes[i].finishTime,
               processes[i].turnaroundTime, processes[i].waitingTime);
    }
}

// Function to sort processes by arrival time
void sortByArrivalTime(struct Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].arrivalTime > processes[j + 1].arrivalTime) {
                struct Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

// Non-Preemptive Priority
void priorityNonPreemptive(struct Process processes[], int n) {
    struct Process result[MAX_PROCESSES];
    int resultSize = 0;
    int currentTime = 0;
    int* completed = (int*)calloc(n, sizeof(int));
    
    sortByArrivalTime(processes, n);
    
    while (resultSize < n) {
        int selectedProcess = -1;
        int highestPriority = 99999;
        
        // Find process with highest priority among arrived processes
        for (int i = 0; i < n; i++) {
            if (!completed[i] && processes[i].arrivalTime <= currentTime) {
                if (processes[i].priority < highestPriority) {
                    highestPriority = processes[i].priority;
                    selectedProcess = i;
                }
            }
        }
        
        if (selectedProcess == -1) {
            currentTime++;
            continue;
        }
        
        // Process the selected process
        result[resultSize] = processes[selectedProcess];
        result[resultSize].finishTime = currentTime + processes[selectedProcess].burstTime;
        result[resultSize].turnaroundTime = result[resultSize].finishTime - 
                                          processes[selectedProcess].arrivalTime;
        result[resultSize].waitingTime = result[resultSize].turnaroundTime - 
                                       processes[selectedProcess].burstTime;
        
        currentTime = result[resultSize].finishTime;
        completed[selectedProcess] = 1;
        resultSize++;
    }
    
    printf("\nPriority (Non-Preemptive):");
    displayResults(result, resultSize);
    free(completed);
}

// Preemptive Priority
void priorityPreemptive(struct Process processes[], int n) {
    struct Process result[MAX_PROCESSES];
    int resultSize = 0;
    int currentTime = 0;
    int completed = 0;
    
    int* remainingBurstTime = (int*)malloc(n * sizeof(int));
    int* isCompleted = (int*)calloc(n, sizeof(int));
    
    // Initialize remaining burst time
    for (int i = 0; i < n; i++) {
        remainingBurstTime[i] = processes[i].burstTime;
    }
    
    while (completed != n) {
        int idx = -1;
        int highestPriority = 99999;
        
        // Find process with highest priority
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && 
                !isCompleted[i] && 
                processes[i].priority < highestPriority) {
                highestPriority = processes[i].priority;
                idx = i;
            }
        }
        
        if (idx != -1) {
            remainingBurstTime[idx]--;
            currentTime++;
            
            if (remainingBurstTime[idx] == 0) {
                result[resultSize] = processes[idx];
                result[resultSize].finishTime = currentTime;
                result[resultSize].turnaroundTime = currentTime - processes[idx].arrivalTime;
                result[resultSize].waitingTime = result[resultSize].turnaroundTime - 
                                               processes[idx].burstTime;
                
                isCompleted[idx] = 1;
                completed++;
                resultSize++;
            }
        } else {
            currentTime++;
        }
    }
    
    printf("\nPriority Scheduling (Preemptive):");
    displayResults(result, resultSize);
    
    free(remainingBurstTime);
    free(isCompleted);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    struct Process processes[MAX_PROCESSES];
    
    // Input process details
    for (int i = 0; i < n; i++) {
        printf("\nEnter arrival time, burst time, and priority for process %d: ", i + 1);
        processes[i].id = i + 1;
        scanf("%d %d %d", &processes[i].arrivalTime, &processes[i].burstTime, &processes[i].priority);
    }
    
    // Create copies of processes array for both scheduling methods
    struct Process nonPreemptiveProcesses[MAX_PROCESSES];
    struct Process preemptiveProcesses[MAX_PROCESSES];
    
    for (int i = 0; i < n; i++) {
        nonPreemptiveProcesses[i] = processes[i];
        preemptiveProcesses[i] = processes[i];
    }
    
    priorityNonPreemptive(nonPreemptiveProcesses, n);
    priorityPreemptive(preemptiveProcesses, n);
    
    return 0;
}