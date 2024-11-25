#include <stdio.h>
#define MAX_PROCESSES 100

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int is_completed;
}Process;

void swap (Process* a, Process* b) {
    Process temp = *a;
    *a = *b;
    *b = temp;
}

void sort_by_arrival_time (Process* processes, int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (processes[j].arrival_time > processes[j+1].arrival_time) {
                swap(&processes[j], &processes[j+1]);
            }
        }
    }
}

void input_processes(Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        processes[i].pid = i+1;
        processes[i].is_completed = 0;
        printf("Enter arrival time for process %d: ", i+1);
        scanf("%d", &processes[i].arrival_time);
        printf("Enter burst time for process %d: ", i+1);
        scanf("%d", &processes[i].burst_time);
    }
}

void print_gantt_chart(int *schedule, int *timeline, int n) {
    printf("\nGantt Chart:\n");

    printf("\n|");
    for (int i = 0; i < n; i++) {
        printf(" P%-2d |", schedule[i]);
    }

    printf("\n");
    for (int i = 0; i <= n; i++) {
        printf("%-3d   ", timeline[i]);
    }
    printf("\n");

}

void print_table (Process* processes, int n) {
    printf("\nProcess Table: \n");
    printf("+---------+---------+---------+---------+---------+---------+\n");
    printf("| Process | Arrival | Burst   | Comple  | TurnAr  | Waiting |\n");
    printf("| ID      | time    | time    | time    | time    | time    |\n");
    printf("+---------+---------+---------+---------+---------+---------+\n");
    for (int i = 0; i < n; i++) {
        printf("| %-7d | %-7d | %-7d | %-7d | %-7d | %-7d |\n", processes[i].pid,
        processes[i].arrival_time, processes[i].burst_time, processes[i].completion_time,
        processes[i].turnaround_time, processes[i].waiting_time);
    }
    printf("+---------+---------+---------+---------+---------+---------+\n");

}

void fcfs (Process* processes, int n) {
    int current_time = 0; 
    int schedule[MAX_PROCESSES * 2];
    int timeline[MAX_PROCESSES * 2 + 1];
    int schedule_index = 0;

    sort_by_arrival_time(processes, n);

    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time > current_time) {
            current_time = processes[i].arrival_time;
        }
        schedule[schedule_index] = processes[i].pid;
        timeline[schedule_index] = current_time;
        schedule_index++;
        processes[i].is_completed = 1;
        processes[i].completion_time = current_time + processes[i].burst_time;
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
        current_time = processes[i].completion_time;
    } 
    timeline[schedule_index] = current_time;  
    print_table(processes, n);
    print_gantt_chart(schedule, timeline, schedule_index);
}

int main() {
    Process processes[MAX_PROCESSES];
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    input_processes(processes, n);
    fcfs(processes, n);

    return 0;
}   


// Enter number of processes: 5
// Enter arrival time for process 1: 0
// Enter burst time for process 1: 4
// Enter arrival time for process 2: 1
// Enter burst time for process 2: 3
// Enter arrival time for process 3: 2
// Enter burst time for process 3: 1
// Enter arrival time for process 4: 3
// Enter burst time for process 4: 2
// Enter arrival time for process 5: 4
// Enter burst time for process 5: 5
