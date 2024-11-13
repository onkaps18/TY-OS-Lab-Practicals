#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

int BUFFER_SIZE;
int *buffer;
int count = 0;
int in = 0, out = 0;
int num_producers, num_consumers;

// Semaphores
sem_t empty;       // Counts empty buffer slots
sem_t full;        // Counts full buffer slots
sem_t mutex;       // Binary semaphore for mutual exclusion

// Function to print buffer status
void print_buffer() {
    printf("\nBuffer Status: [");
    for(int i = 0; i < BUFFER_SIZE; i++) {
        if(i < count) {
            printf("%d", buffer[(out + i) % BUFFER_SIZE]);
        } else {
            printf("_");
        }
        if(i < BUFFER_SIZE - 1) printf(",");
    }
    printf("] (Count: %d)\n", count);
}

void *producer(void *arg) {
    int id = *((int *)arg);
    int item;
    
    while(1) {
        printf("\nProducer %d: Waiting for empty slot\n", id);
        sem_wait(&empty);  // Wait for empty slot
        
        printf("Producer %d: Waiting to enter critical section\n", id);
        sem_wait(&mutex);  // Enter critical section
        
        printf("Producer %d: Entered critical section\n", id);
        
        // Produce item
        item = (id * 100) + (rand() % 100);
        buffer[in] = item;
        printf("Producer %d: Produced item %d at position %d\n", id, item, in);
        in = (in + 1) % BUFFER_SIZE;
        count++;
        
        print_buffer();
        
        printf("Producer %d: Leaving critical section\n", id);
        sem_post(&mutex);  // Exit critical section
        
        sem_post(&full);   // Signal that a new item is in buffer
        
        sleep(rand() % 3 + 1);  // Random sleep between 1-3 seconds
    }
    return NULL;
}

void *consumer(void *arg) {
    int id = *((int *)arg);
    int item;
    
    while(1) {
        printf("\nConsumer %d: Waiting for item\n", id);
        sem_wait(&full);   // Wait for available item
        
        printf("Consumer %d: Waiting to enter critical section\n", id);
        sem_wait(&mutex);  // Enter critical section
        
        printf("Consumer %d: Entered critical section\n", id);
        
        // Consume item
        item = buffer[out];
        printf("Consumer %d: Consumed item %d from position %d\n", id, item, out);
        out = (out + 1) % BUFFER_SIZE;
        count--;
        
        print_buffer();
        
        printf("Consumer %d: Leaving critical section\n", id);
        sem_post(&mutex);  // Exit critical section
        
        sem_post(&empty);  // Signal that a buffer slot is free
        
        sleep(rand() % 4 + 2);  // Random sleep between 2-5 seconds
    }
    return NULL;
}

int main() {
    // Get buffer size and number of producers/consumers from user
    do {
        printf("Enter buffer size (must be greater than 0): ");
        scanf("%d", &BUFFER_SIZE);
    } while(BUFFER_SIZE <= 0);

    printf("Enter number of producers: ");
    scanf("%d", &num_producers);
    
    printf("Enter number of consumers: ");
    scanf("%d", &num_consumers);

    // Dynamically allocate buffer
    buffer = (int *)malloc(BUFFER_SIZE * sizeof(int));
    if(buffer == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Initialize random seed
    srand(time(NULL));

    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);  // Initially all slots are empty
    sem_init(&full, 0, 0);             // Initially no items in buffer
    sem_init(&mutex, 0, 1);            // Binary semaphore for mutual exclusion

    // Create producer and consumer threads
    pthread_t *prod_threads = malloc(num_producers * sizeof(pthread_t));
    pthread_t *cons_threads = malloc(num_consumers * sizeof(pthread_t));
    int *prod_ids = malloc(num_producers * sizeof(int));
    int *cons_ids = malloc(num_consumers * sizeof(int));

    printf("\nStarting Producer-Consumer Simulation...\n");
    printf("Buffer Size: %d\n", BUFFER_SIZE);
    printf("Number of Producers: %d\n", num_producers);
    printf("Number of Consumers: %d\n\n", num_consumers);

    // Create producer threads
    for(int i = 0; i < num_producers; i++) {
        prod_ids[i] = i + 1;
        pthread_create(&prod_threads[i], NULL, producer, &prod_ids[i]);
    }

    // Create consumer threads
    for(int i = 0; i < num_consumers; i++) {
        cons_ids[i] = i + 1;
        pthread_create(&cons_threads[i], NULL, consumer, &cons_ids[i]);
    }

    // Wait for threads (note: in this version, threads run indefinitely)
    for(int i = 0; i < num_producers; i++) {
        pthread_join(prod_threads[i], NULL);
    }
    for(int i = 0; i < num_consumers; i++) {
        pthread_join(cons_threads[i], NULL);
    }

    // Cleanup
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);
    free(buffer);
    free(prod_threads);
    free(cons_threads);
    free(prod_ids);
    free(cons_ids);

    return 0;
}