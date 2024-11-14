#include <stdio.h>
#include <stdlib.h>

void printTable(int request_queue[], int n, int head_start) {
    printf("\nRequest Sequence Table:\n");
    printf("+---------------+------------------+--------------------+\n");
    printf("| Track Number  | Head Movement    | Seek Distance     |\n");
    printf("+---------------+------------------+--------------------+\n");
    
    int current_track = head_start;
    
    for(int i = 0; i < n; i++) {
        int seek = abs(request_queue[i] - current_track);
        printf("| %-13d | %-16d | %-18d |\n", 
               request_queue[i], 
               current_track, 
               seek);
        current_track = request_queue[i];
    }
    printf("+---------------+------------------+--------------------+\n");
}

int main() {
    int n, head_start;
    
    // Get the number of tracks to be accessed
    printf("Enter the number of tracks to access: ");
    scanf("%d", &n);
    
    // Dynamically allocate memory for request queue
    int *request_queue = (int *)malloc(n * sizeof(int));
    
    // Get the track numbers
    printf("Enter the track numbers in request queue:\n");
    for(int i = 0; i < n; i++) {
        printf("Track %d: ", i + 1);
        scanf("%d", &request_queue[i]);
    }
    
    // Get the initial head position
    printf("Enter the initial position of R/W head: ");
    scanf("%d", &head_start);
    
    // Calculate total seek time
    int total_seek_time = 0;
    int current_track = head_start;
    
    // Print the initial condition
    printf("\nInitial head position: %d\n", head_start);
    
    // Print the sequence table
    printTable(request_queue, n, head_start);
    
    // Calculate and sum up the seek time
    for(int i = 0; i < n; i++) {
        total_seek_time += abs(request_queue[i] - current_track);
        current_track = request_queue[i];
    }
    
    // Print the results
    printf("\nTotal Seek Time: %d\n", total_seek_time);
    printf("Average Seek Time: %.2f\n", (float)total_seek_time/n);
    
    // Free allocated memory
    free(request_queue);
    
    return 0;
}

// Enter the number of tracks to access: 7
// Enter the track numbers in request queue:
// Track 1: 82
// Track 2: 170
// Track 3: 43
// Track 4: 140
// Track 5: 24
// Track 6: 16
// Track 7: 190
// Enter the initial position of R/W head: 50
