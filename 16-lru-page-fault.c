#include <stdio.h>
#include <string.h>

#define MAX_FRAMES 10

// Function to find the least recently used page
int find_least_recently_used(int last_used[], int num_frames) {
    int min_index = 0;
    for (int i = 1; i < num_frames; i++) {
        if (last_used[i] < last_used[min_index]) {
            min_index = i;
        }
    }
    return min_index;
}

void start_execution(int num_frames) {
    char reference_string[100];
    char frames[MAX_FRAMES];
    int last_used[MAX_FRAMES];  // Tracks the last used time for each frame
    int page_faults = 0;
    int time = 0;

    printf("Enter the reference string: ");
    scanf(" %[^\n]s", reference_string);

    // Initialize frames and last_used times
    for (int i = 0; i < num_frames; i++) {
        frames[i] = '\0';
        last_used[i] = -1;  // -1 means the frame is empty
    }

    for (int i = 0; reference_string[i] != '\0'; i++) {
        char current_page = reference_string[i];
        int found = 0;

        // Check if the page is already in frames
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == current_page) {
                found = 1;
                last_used[j] = time++;  // Update last used time
                printf("%s -> hit\n", frames);
                break;
            }
        }

        // If the page is not found, replace the least recently used page
        if (!found) {
            page_faults++;

            // Find an empty frame or the least recently used frame to replace
            int replace_index = -1;
            for (int j = 0; j < num_frames; j++) {
                if (frames[j] == '\0') {
                    replace_index = j;
                    break;
                }
            }

            // If no empty frame found, use the least recently used page
            if (replace_index == -1) {
                replace_index = find_least_recently_used(last_used, num_frames);
            }

            // Replace the page and update last used time
            frames[replace_index] = current_page;
            last_used[replace_index] = time++;
            printf("%s \n", frames);
        }
    }

    printf("Total number of page faults: %d\n", page_faults);
}

int main() {
    int num_frames;

    printf("Enter the total number of frames: ");
    scanf("%d", &num_frames);

    if (num_frames > MAX_FRAMES) {
        printf("Error: Number of frames exceeds the maximum limit.\n");
        return 1;
    }

    start_execution(num_frames);

    return 0;
}
//sampple input = 3213416243421452134