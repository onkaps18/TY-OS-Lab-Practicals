#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Function to sort the array
void bubbleSort(int arr[], int n) {
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void printTable(int sequence[], int seek_distances[], int sequence_size, int head_start) {
    printf("\nRequest Sequence Table:\n");
    printf("+---------------+------------------+--------------------+\n");
    printf("| Track Number  | Head Movement    | Seek Distance     |\n");
    printf("+---------------+------------------+--------------------+\n");
    
    int current_track = head_start;
    
    for(int i = 0; i < sequence_size; i++) {
        printf("| %-13d | %-16d | %-18d |\n", 
               sequence[i], 
               current_track, 
               seek_distances[i]);
        current_track = sequence[i];
    }
    printf("+---------------+------------------+--------------------+\n");
}

int main() {
    int n, head_start, disk_size;
    char direction;
    
    // Get the number of tracks to be accessed
    printf("Enter the number of tracks to access: ");
    scanf("%d", &n);
    
    // Get disk size
    printf("Enter the size of the disk (number of tracks): ");
    scanf("%d", &disk_size);
    disk_size--; // Convert to 0-based indexing
    
    // Dynamically allocate memory
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
    
    // Get the direction of head movement
    printf("Enter the direction of head movement (l for left, r for right): ");
    scanf(" %c", &direction);
    
    // Sort the request array
    bubbleSort(request_queue, n);
    
    // Find the index of first track that is greater than head position
    int index = 0;
    for(int i = 0; i < n; i++) {
        if(request_queue[i] >= head_start) {
            index = i;
            break;
        }
    }
    
    // Arrays to store sequence and seek distances
    int *sequence = (int *)malloc((n + 2) * sizeof(int));  // +2 for possible 0 and disk_size
    int *seek_distances = (int *)malloc((n + 2) * sizeof(int));
    int sequence_size = 0;
    int current_track = head_start;
    int total_seek_time = 0;
    
    // SCAN algorithm implementation
    if(direction == 'r') {  // Moving right
        // Service tracks to the right of head
        for(int i = index; i < n; i++) {
            sequence[sequence_size] = request_queue[i];
            seek_distances[sequence_size] = abs(request_queue[i] - current_track);
            total_seek_time += seek_distances[sequence_size];
            current_track = request_queue[i];
            sequence_size++;
        }
        
        // If there are tracks to the left, go to end and then left
        if(index > 0) {
            // Go to the end
            sequence[sequence_size] = disk_size;
            seek_distances[sequence_size] = abs(disk_size - current_track);
            total_seek_time += seek_distances[sequence_size];
            current_track = disk_size;
            sequence_size++;
            
            // Service tracks to the left
            for(int i = index-1; i >= 0; i--) {
                sequence[sequence_size] = request_queue[i];
                seek_distances[sequence_size] = abs(request_queue[i] - current_track);
                total_seek_time += seek_distances[sequence_size];
                current_track = request_queue[i];
                sequence_size++;
            }
        }
    }
    else {  // Moving left
        // Service tracks to the left of head
        for(int i = index-1; i >= 0; i--) {
            sequence[sequence_size] = request_queue[i];
            seek_distances[sequence_size] = abs(request_queue[i] - current_track);
            total_seek_time += seek_distances[sequence_size];
            current_track = request_queue[i];
            sequence_size++;
        }
        
        // If there are tracks to the right, go to 0 and then right
        if(index < n) {
            // Go to the beginning
            sequence[sequence_size] = 0;
            seek_distances[sequence_size] = abs(0 - current_track);
            total_seek_time += seek_distances[sequence_size];
            current_track = 0;
            sequence_size++;
            
            // Service tracks to the right
            for(int i = index; i < n; i++) {
                sequence[sequence_size] = request_queue[i];
                seek_distances[sequence_size] = abs(request_queue[i] - current_track);
                total_seek_time += seek_distances[sequence_size];
                current_track = request_queue[i];
                sequence_size++;
            }
        }
    }
    
    // Print the initial condition
    printf("\nInitial head position: %d", head_start);
    printf("\nDirection: %s", (direction == 'r') ? "right" : "left");
    
    // Print the sequence table
    printTable(sequence, seek_distances, sequence_size, head_start);
    
    // Print final results
    printf("\nTotal Seek Time: %d\n", total_seek_time);
    printf("Average Seek Time: %.2f\n", (float)total_seek_time/n);
    
    // Free allocated memory
    free(request_queue);
    free(sequence);
    free(seek_distances);
    
    return 0;
}