#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

void printTable(int sequence[], int seek_distances[], int n, int head_start) {
    printf("\nRequest Sequence Table:\n");
    printf("+---------------+------------------+--------------------+\n");
    printf("| Track Number  | Head Movement    | Seek Distance     |\n");
    printf("+---------------+------------------+--------------------+\n");
    
    int current_track = head_start;
    
    for(int i = 0; i < n; i++) {
        printf("| %-13d | %-16d | %-18d |\n", 
               sequence[i], 
               current_track, 
               seek_distances[i]);
        current_track = sequence[i];
    }
    printf("+---------------+------------------+--------------------+\n");
}

// Function to find the track with minimum seek time
int findMinSeekTime(int* request_queue, bool* visited, int n, int head_pos) {
    int min_seek = INT_MAX;
    int min_index = -1;
    
    for(int i = 0; i < n; i++) {
        if(!visited[i]) {
            int seek = abs(request_queue[i] - head_pos);
            if(seek < min_seek) {
                min_seek = seek;
                min_index = i;
            }
        }
    }
    return min_index;
}

int main() {
    int n, head_start;
    
    // Get the number of tracks to be accessed
    printf("Enter the number of tracks to access: ");
    scanf("%d", &n);
    
    // Dynamically allocate memory
    int *request_queue = (int *)malloc(n * sizeof(int));
    int *sequence = (int *)malloc(n * sizeof(int));
    int *seek_distances = (int *)malloc(n * sizeof(int));
    bool *visited = (bool *)calloc(n, sizeof(bool));  // Initialize all to false
    
    // Get the track numbers
    printf("Enter the track numbers in request queue:\n");
    for(int i = 0; i < n; i++) {
        printf("Track %d: ", i + 1);
        scanf("%d", &request_queue[i]);
    }
    
    // Get the initial head position
    printf("Enter the initial position of R/W head: ");
    scanf("%d", &head_start);
    
    // Initialize variables for SSTF algorithm
    int current_track = head_start;
    int total_seek_time = 0;
    
    // Print the initial condition
    printf("\nInitial head position: %d", head_start);
    
    // SSTF algorithm implementation
    for(int i = 0; i < n; i++) {
        // Find the track with minimum seek time
        int next_index = findMinSeekTime(request_queue, visited, n, current_track);
        
        // Mark this track as visited
        visited[next_index] = true;
        
        // Calculate seek distance
        int seek_distance = abs(request_queue[next_index] - current_track);
        
        // Store the values for table printing
        sequence[i] = request_queue[next_index];
        seek_distances[i] = seek_distance;
        
        // Update current track and total seek time
        current_track = request_queue[next_index];
        total_seek_time += seek_distance;
    }
    
    // Print the sequence table
    printTable(sequence, seek_distances, n, head_start);
    
    // Print final results
    printf("\nTotal Seek Time: %d\n", total_seek_time);
    printf("Average Seek Time: %.2f\n", (float)total_seek_time/n);
    
    // Free allocated memory
    free(request_queue);
    free(sequence);
    free(seek_distances);
    free(visited);
    
    return 0;
}

// Track 1: 82
// Track 2: 170
// Track 3: 43
// Track 4: 140
// Track 5: 24
// Track 6: 16
// Track 7: 190
// Enter the initial position of R/W head: 50
