#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

// Global variables
int shared_resource = 0;
int reader_count = 0;

// Mutex declarations
pthread_mutex_t resource_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t reader_count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

// Configuration variables
int num_readers;
int num_writers;
int reader_sleep_time;
int writer_sleep_time;

// Function for synchronized printing
void print_status(const char* format, ...) {
    va_list args;
    pthread_mutex_lock(&print_mutex);
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    fflush(stdout);
    pthread_mutex_unlock(&print_mutex);
}

// Reader thread function
void* reader(void* arg) {
    int id = *(int*)arg;
    while (1) {
        // Entry section
        pthread_mutex_lock(&reader_count_mutex);
        reader_count++;
        print_status("Reader %d: Entering critical section (Total readers: %d)\n", 
                    id, reader_count);
        
        if (reader_count == 1) {
            pthread_mutex_lock(&resource_mutex);
            print_status("Reader %d: First reader, locked resource\n", id);
        }
        pthread_mutex_unlock(&reader_count_mutex);

        // Critical section
        print_status("Reader %d: Reading shared value: %d\n", 
                    id, shared_resource);
        sleep(1);  // Simulate reading

        // Exit section
        pthread_mutex_lock(&reader_count_mutex);
        reader_count--;
        print_status("Reader %d: Leaving critical section (Remaining readers: %d)\n", 
                    id, reader_count);
        
        if (reader_count == 0) {
            pthread_mutex_unlock(&resource_mutex);
            print_status("Reader %d: Last reader, released resource\n", id);
        }
        pthread_mutex_unlock(&reader_count_mutex);

        // Rest section
        print_status("Reader %d: Going to sleep for %d seconds\n", 
                    id, reader_sleep_time);
        sleep(reader_sleep_time);
    }
    return NULL;
}

// Writer thread function
void* writer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        // Entry section
        print_status("Writer %d: Waiting for resource access\n", id);
        pthread_mutex_lock(&resource_mutex);
        print_status("Writer %d: Entered critical section\n", id);

        // Critical section
        shared_resource++;
        print_status("Writer %d: Writing new value: %d\n", 
                    id, shared_resource);
        sleep(2);  // Simulate writing

        // Exit section
        print_status("Writer %d: Leaving critical section\n", id);
        pthread_mutex_unlock(&resource_mutex);

        // Rest section
        print_status("Writer %d: Going to sleep for %d seconds\n", 
                    id, writer_sleep_time);
        sleep(writer_sleep_time);
    }
    return NULL;
}

int main() {
    printf("=== Readers-Writers Problem Simulation ===\n\n");
    
    // Get user input
    printf("Enter number of readers: ");
    scanf("%d", &num_readers);
    
    printf("Enter number of writers: ");
    scanf("%d", &num_writers);
    
    printf("Enter reader sleep time in seconds: ");
    scanf("%d", &reader_sleep_time);
    
    printf("Enter writer sleep time in seconds: ");
    scanf("%d", &writer_sleep_time);

    printf("\nStarting simulation with:\n");
    printf("Readers: %d\n", num_readers);
    printf("Writers: %d\n", num_writers);
    printf("Reader sleep time: %d seconds\n", reader_sleep_time);
    printf("Writer sleep time: %d seconds\n\n", writer_sleep_time);

    // Thread creation arrays
    pthread_t readers[num_readers], writers[num_writers];
    int reader_ids[num_readers], writer_ids[num_writers];

    // Create reader threads
    for (int i = 0; i < num_readers; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Create writer threads
    for (int i = 0; i < num_writers; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Join reader threads
    for (int i = 0; i < num_readers; i++) {
        pthread_join(readers[i], NULL);
    }

    // Join writer threads
    for (int i = 0; i < num_writers; i++) {
        pthread_join(writers[i], NULL);
    }

    // Cleanup mutexes
    pthread_mutex_destroy(&resource_mutex);
    pthread_mutex_destroy(&reader_count_mutex);
    pthread_mutex_destroy(&print_mutex);

    return 0;
}

// Enter number of readers: 2
// Enter number of writers: 1
// Enter reader sleep time in seconds: 2
// Enter writer sleep time in seconds: 3