#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

// Global variables
int shared_resource = 0;
int reader_count = 0;

// Semaphore declarations
sem_t resource_sem;      // Controls access to shared resource
sem_t reader_count_sem;  // Controls access to reader_count
sem_t print_sem;         // Controls printing

// Configuration variables
int num_readers;
int num_writers;
int reader_sleep_time;
int writer_sleep_time;

// Function for synchronized printing
void print_status(const char* format, ...) {
    va_list args;
    sem_wait(&print_sem);
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    fflush(stdout);
    sem_post(&print_sem);
}

// Reader thread function
void* reader(void* arg) {
    int id = *(int*)arg;
    while (1) {
        // Entry section
        sem_wait(&reader_count_sem);
        reader_count++;
        print_status("Reader %d: Entering critical section (Total readers: %d)\n", 
                    id, reader_count);
        
        if (reader_count == 1) {
            // First reader must wait for resource
            sem_wait(&resource_sem);
            print_status("Reader %d: First reader, acquired resource\n", id);
        }
        sem_post(&reader_count_sem);

        // Critical section
        print_status("Reader %d: Reading shared value: %d\n", 
                    id, shared_resource);
        sleep(1);  // Simulate reading

        // Exit section
        sem_wait(&reader_count_sem);
        reader_count--;
        print_status("Reader %d: Leaving critical section (Remaining readers: %d)\n", 
                    id, reader_count);
        
        if (reader_count == 0) {
            // Last reader releases the resource
            sem_post(&resource_sem);
            print_status("Reader %d: Last reader, released resource\n", id);
        }
        sem_post(&reader_count_sem);

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
        sem_wait(&resource_sem);
        print_status("Writer %d: Entered critical section\n", id);

        // Critical section
        shared_resource++;
        print_status("Writer %d: Writing new value: %d\n", 
                    id, shared_resource);
        sleep(2);  // Simulate writing

        // Exit section
        print_status("Writer %d: Leaving critical section\n", id);
        sem_post(&resource_sem);

        // Rest section
        print_status("Writer %d: Going to sleep for %d seconds\n", 
                    id, writer_sleep_time);
        sleep(writer_sleep_time);
    }
    return NULL;
}

int main() {
    printf("=== Readers-Writers Problem Simulation (Semaphore Version) ===\n\n");
    
    // Initialize semaphores
    sem_init(&resource_sem, 0, 1);     // Binary semaphore for resource
    sem_init(&reader_count_sem, 0, 1); // Binary semaphore for reader count
    sem_init(&print_sem, 0, 1);        // Binary semaphore for printing
    
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

    // Cleanup semaphores
    sem_destroy(&resource_sem);
    sem_destroy(&reader_count_sem);
    sem_destroy(&print_sem);

    return 0;
}