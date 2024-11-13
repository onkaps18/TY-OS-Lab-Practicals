#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FRAMES 100
#define MAX_REFERENCE_LENGTH 100

char* referenceString;

char searchPageToBeReplaced(int index, char frames[], int frameCount) {
    int storeIndex[MAX_FRAMES];
    int storeIndexCount = 0;

    for (int i = 0; i < frameCount; i++) {
        int usedInFuture = 0;
        for (int j = index; referenceString[j] != '\0'; j++) {
            if (frames[i] == referenceString[j]) {
                usedInFuture = 1;
                storeIndex[storeIndexCount++] = j;
                break;
            }
        }
        if (!usedInFuture) {
            return frames[i];
        }
    }

    int maxValue = -1;
    int indexOfMaxValue = 0;
    for (int i = 0; i < storeIndexCount; i++) {
        if (storeIndex[i] > maxValue) {
            maxValue = storeIndex[i];
            indexOfMaxValue = i;
        }
    }
    return frames[indexOfMaxValue];
}

void implementOptimal(int totalNumberOfFrames) {
    char frames[MAX_FRAMES];
    memset(frames, 0, sizeof(frames));
    int frameCount = 0;
    int totalNumberOfPageFaults = 0;

    for (int i = 0; referenceString[i] != '\0'; i++) {
        int found = 0;
        for (int j = 0; j < frameCount; j++) {
            if (frames[j] == referenceString[i]) {
                found = 1;
                printf("[");
                for (int k = 0; k < frameCount; k++) {
                    printf("%c", frames[k]);
                }
                printf("] -> hit\n");
                break;
            }
        }

        if (!found) {
            if (frameCount < totalNumberOfFrames) {
                frames[frameCount++] = referenceString[i];
                totalNumberOfPageFaults++;
                printf("[");
                for (int k = 0; k < frameCount; k++) {
                    printf("%c", frames[k]);
                }
                printf("]\n");
            } else {
                char replaceThisPage = searchPageToBeReplaced(i + 1, frames, frameCount);
                for (int j = 0; j < frameCount; j++) {
                    if (frames[j] == replaceThisPage) {
                        frames[j] = referenceString[i];
                        break;
                    }
                }
                totalNumberOfPageFaults++;
                printf("[");
                for (int k = 0; k < frameCount; k++) {
                    printf("%c", frames[k]);
                }
                printf("]\n");
            }
        }
    }

    printf("Total number of page faults: %d\n", totalNumberOfPageFaults);
}

int main() {
    int totalNumberOfFrames;
    printf("Enter total number of frames: ");
    scanf("%d", &totalNumberOfFrames);

    // Allocate memory for referenceString
    referenceString = (char*)malloc(MAX_REFERENCE_LENGTH * sizeof(char));
    if (referenceString == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Enter the reference string: ");
    scanf(" %[^\n]s", referenceString);

    implementOptimal(totalNumberOfFrames);

    // Free the allocated memory
    free(referenceString);

    return 0;
}
