#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 100

// Process structure
struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int is_completed;
};

// Queue structure
struct Queue {
    int items[MAX_QUEUE_SIZE];
    int front;
    int rear;
};

// Queue operations
void initQueue(struct Queue *q) {
    q->front = -1;
    q->rear = -1;
}

int isEmpty(struct Queue *q) {
    return (q->front == -1);
}

void enqueue(struct Queue *q, int value) {
    if (q->front == -1) {
        q->front = 0;
    }
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->items[q->rear] = value;
}

int dequeue(struct Queue *q) {
    int value = q->items[q->front];
    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    } else {
        q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    }
    return value;
}

void roundRobinScheduling(struct Process processes[], int n, int quantum) {
    struct Queue readyQueue;
    initQueue(&readyQueue);
    
    int current_time = 0;
    int completed = 0;
    int *remaining_time = (int *)malloc(n * sizeof(int));
    int *enqueued = (int *)malloc(n * sizeof(int));
    
    // Initialize arrays
    for(int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].burst_time;
        processes[i].is_completed = 0;
        enqueued[i] = 0;
    }
    
    // Main scheduling loop
    while(completed != n) {
        // Check for newly arrived processes
        for(int i = 0; i < n; i++) {
            if(processes[i].arrival_time <= current_time && !enqueued[i] && !processes[i].is_completed) {
                enqueue(&readyQueue, i);
                enqueued[i] = 1;
            }
        }
        
        // If queue is empty but processes remain
        if(isEmpty(&readyQueue)) {
            current_time++;
            continue;
        }
        
        // Process the front of queue
        int current_process = dequeue(&readyQueue);
        
        // Execute for quantum time or remaining time
        int exec_time = (remaining_time[current_process] < quantum) ? 
                        remaining_time[current_process] : quantum;
        
        remaining_time[current_process] -= exec_time;
        current_time += exec_time;
        
        // Check for new arrivals during execution
        for(int i = 0; i < n; i++) {
            if(processes[i].arrival_time <= current_time && !enqueued[i] && !processes[i].is_completed) {
                enqueue(&readyQueue, i);
                enqueued[i] = 1;
            }
        }
        
        // If process is not completed, re-enqueue it
        if(remaining_time[current_process] > 0) {
            enqueue(&readyQueue, current_process);
        }
        // If process is completed
        else {
            processes[current_process].completion_time = current_time;
            processes[current_process].turnaround_time = 
                processes[current_process].completion_time - processes[current_process].arrival_time;
            processes[current_process].waiting_time = 
                processes[current_process].turnaround_time - processes[current_process].burst_time;
            processes[current_process].is_completed = 1;
            completed++;
        }
    }
    
    free(remaining_time);
    free(enqueued);
}

void printTable(struct Process processes[], int n) {
    printf("\nProcess Scheduling Table:\n");
    printf("+---------+---------------+------------+------------------+------------------+---------------+\n");
    printf("| PID     | Arrival Time  | Burst Time | Completion Time | Turnaround Time | Waiting Time |\n");
    printf("+---------+---------------+------------+------------------+------------------+---------------+\n");
    
    float total_turnaround = 0, total_waiting = 0;
    
    for(int i = 0; i < n; i++) {
        printf("| %-7d | %-13d | %-10d | %-16d | %-16d | %-13d |\n",
            processes[i].pid,
            processes[i].arrival_time,
            processes[i].burst_time,
            processes[i].completion_time,
            processes[i].turnaround_time,
            processes[i].waiting_time
        );
        
        total_turnaround += processes[i].turnaround_time;
        total_waiting += processes[i].waiting_time;
    }
    
    printf("+---------+---------------+------------+------------------+------------------+---------------+\n");
    
    printf("\nAverage Turnaround Time: %.2f\n", total_turnaround/n);
    printf("Average Waiting Time: %.2f\n", total_waiting/n);
}

int main() {
    int n, quantum;
    
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    printf("Enter time quantum: ");
    scanf("%d", &quantum);
    
    struct Process processes[n];
    
    // Input process details
    for(int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("\nEnter details for Process %d:\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &processes[i].arrival_time);
        printf("Burst Time: ");
        scanf("%d", &processes[i].burst_time);
    }
    
    roundRobinScheduling(processes, n, quantum);
    printTable(processes, n);
    
    return 0;
}
// Sample input : 
// Enter the number of processes: 6
// Enter time quantum: 3
// 
// Enter details for Process 1:
// Arrival Time: 0
// Burst Time: 4
// 
// Enter details for Process 2:
// Arrival Time: 1
// Burst Time: 5
// 
// Enter details for Process 3:
// Arrival Time: 2
// Burst Time: 2
// 
// Enter details for Process 4:
// Arrival Time: 3
// Burst Time: 1
// 
// Enter details for Process 5:
// Arrival Time: 4
// Burst Time: 6
// 
// Enter details for Process 6:
// Arrival Time: 6
// Burst Time: 3

