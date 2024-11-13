#include <stdio.h>
#include <limits.h>
#define MAX_PROCESSES 100

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int is_completed;
    int remaining_time;
}Process;

void input_processes (Process* p, int n) {
    for (int i = 0; i < n; i++) {
        p[i].pid = i+1;
        p[i].is_completed = 0;
        printf("Enter arrival time for process %d: ", i+1);
        scanf("%d", &p[i].arrival_time);
        printf("Enter burst time for process %d: ", i+1);
        scanf("%d", &p[i].burst_time);
        p[i].remaining_time = p[i].burst_time;

    }
}

void swap (Process* a, Process* b) {
    Process temp = *a;
    *a = *b;
    *b = temp;
}

void sort_by_arrival_time (Process* p, int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (p[j].arrival_time > p[j+1].arrival_time) {
                swap(&p[j], &p[j+1]);
            }
        }
    }
}

void print_table (Process* p, int n) {
    printf("\nProcess Table: \n");
    printf("+---------+---------+---------+---------+---------+---------+\n");
    printf("| Process | Arrival | Burst   | Comple  | TurnAr  | Waiting |\n");
    printf("| ID      | time    | time    | time    | time    | time    |\n");
    printf("+---------+---------+---------+---------+---------+---------+\n");
    for (int i = 0; i < n; i++) {
        printf("| %-7d | %-7d | %-7d | %-7d | %-7d | %-7d |\n", p[i].pid,
        p[i].arrival_time, p[i].burst_time, p[i].completion_time,
        p[i].turnaround_time, p[i].waiting_time);
    }
}

void sjf (Process* p, int n) {
    int current_time = 0;
    int schedule[MAX_PROCESSES * 2];
    int timeline[MAX_PROCESSES * 2 + 1];
    int schedule_index = 0;
    int completed = 0;

    sort_by_arrival_time(p, n);

    while (completed < n) {
        int shortest_job_index = -1;
        int shortest_job_burst_time = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].remaining_time > 0) {
                if (p[i].remaining_time < shortest_job_burst_time) {
                    shortest_job_index = i;
                    shortest_job_burst_time = p[i].remaining_time;
                }
            }
        }

        if (shortest_job_index == -1) {
            current_time++;
            continue;
        }

        schedule[schedule_index] = p[shortest_job_index].pid;
        timeline[schedule_index] = current_time;
        schedule_index++;

        p[shortest_job_index].completion_time = current_time + p[shortest_job_index].remaining_time;
        current_time = p[shortest_job_index].completion_time;
        p[shortest_job_index].remaining_time = 0;
        p[shortest_job_index].is_completed = 1;
        p[shortest_job_index].turnaround_time = p[shortest_job_index].completion_time - p[shortest_job_index].arrival_time;
        p[shortest_job_index].waiting_time = p[shortest_job_index].turnaround_time - p[shortest_job_index].burst_time;
        completed++;
    }
    timeline[schedule_index] = current_time;

}

void srtf (Process* p, int n) {
    int current_time = 0;
    int schedule[MAX_PROCESSES * 2];
    int timeline[MAX_PROCESSES * 2 + 1];
    int schedule_index = 0;
    int completed = 0;
    int prev_pid = -1;

    sort_by_arrival_time(p, n);

    while (completed < n) {
        int shortest_job_index = -1;
        int shortest_job_remaining_time = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].remaining_time > 0) {
                if (p[i].remaining_time < shortest_job_remaining_time) {
                    shortest_job_index = i;
                    shortest_job_remaining_time = p[i].remaining_time;
                }
            }
        }

        if (shortest_job_index == -1) {
            current_time++;
            continue;;
        }

        if (p[shortest_job_index].is_completed == 0) {
            schedule[schedule_index] = p[shortest_job_index].pid;
            timeline[schedule_index] = current_time;
            schedule_index++;
            prev_pid = p[shortest_job_index].pid;
        }

        p[shortest_job_index].remaining_time--;
        current_time++;

        if (p[shortest_job_index].remaining_time == 0) {
            completed++;
            p[shortest_job_index].completion_time = current_time;
            p[shortest_job_index].turnaround_time = p[shortest_job_index].completion_time - p[shortest_job_index].arrival_time;
            p[shortest_job_index].waiting_time = p[shortest_job_index].turnaround_time - p[shortest_job_index].burst_time;
        }

    }
}


int main() {
    Process processes[MAX_PROCESSES];
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    input_processes(processes, n);


    // sjf(processes, n);
    srtf(processes, n);
    print_table(processes, n);

    return 0;
}

// Process Table: SRTF
// +---------+---------+---------+---------+---------+---------+
// | Process | Arrival | Burst   | Comple  | TurnAr  | Waiting |
// | ID      | time    | time    | time    | time    | time    |
// +---------+---------+---------+---------+---------+---------+
// | 1       | 0       | 7       | 19      | 19      | 12      |
// | 2       | 1       | 5       | 13      | 12      | 7       |
// | 3       | 2       | 3       | 6       | 4       | 1       |
// | 4       | 3       | 1       | 4       | 1       | 0       |
// | 5       | 4       | 2       | 9       | 5       | 3       |
// | 6       | 5       | 1       | 7       | 2       | 1       |


// Process Table: SJF
// +---------+---------+---------+---------+---------+---------+
// | Process | Arrival | Burst   | Comple  | TurnAr  | Waiting |
// | ID      | time    | time    | time    | time    | time    |
// +---------+---------+---------+---------+---------+---------+
// | 1       | 2       | 3       | 5       | 3       | 0       |
// | 2       | 3       | 2       | 7       | 4       | 2       |
// | 3       | 4       | 3       | 13      | 9       | 6       |
// | 4       | 6       | 1       | 8       | 2       | 1       |
// | 5       | 8       | 2       | 10      | 2       | 0       |

// void print_gantt_chart(int *schedule, int *timeline, int n) {
//     printf("\nGantt Chart:\n");

//     printf("\n|");
//     for (int i = 0; i < n; i++) {
//         printf(" P%-2d |", schedule[i]);
//     }

//     printf("\n");
//     for (int i = 0; i <= n; i++) {
//         printf("%-3d   ", timeline[i]);
//     }
// }
