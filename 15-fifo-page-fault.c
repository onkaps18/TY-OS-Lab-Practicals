#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 10

int main() {
    int num_frames, page_faults = 0;
    char reference_string[100];
    char frames[MAX_FRAMES];
    int index = 0;
    int i, j, found;

    // Get the number of frames from the user
    printf("Enter the total number of frames: ");
    scanf("%d", &num_frames);

    // Get the reference string from the user
    printf("Enter the reference string: ");
    scanf(" %[^\n]s", reference_string);

    // Initialize the frames to empty
    for (i = 0; i < MAX_FRAMES; i++) {
        frames[i] = '\0';
    }

    // Simulate the FIFO page replacement algorithm
    for (i = 0; reference_string[i] != '\0'; i++) {
        found = 0;

        // Check if the page is already in the frames
        for (j = 0; j < num_frames; j++) {
            if (frames[j] == reference_string[i]) {
                found = 1;
                printf("%s -> hit\n", frames);
                break;
            }
        }

        // If the page is not found, replace the oldest page
        if (!found) {
            page_faults++;
            if (frames[index] == '\0') {
                frames[index] = reference_string[i];
            } else {
                frames[index] = reference_string[i];
            }
            index = (index + 1) % num_frames;
            printf("%s\n", frames);
        }
    }

    printf("Total number of page faults: %d\n", page_faults);

    return 0;
}

// sample input = 3213416243421452134