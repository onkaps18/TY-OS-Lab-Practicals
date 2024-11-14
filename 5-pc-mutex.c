#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Structure to hold buffer parameters
typedef struct {
    int* items;
    int size;
    int front;
    int rear;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t can_produce;
    pthread_cond_t can_consume;
} CircularBuffer;

// Structure to hold thread parameters
typedef struct {
    int id;
    CircularBuffer* buffer;
} ThreadParams;

// Function to initialize the circular buffer
CircularBuffer* initBuffer(int size) {
    CircularBuffer* buffer = (CircularBuffer*)malloc(sizeof(CircularBuffer));
    if (!buffer) {
        perror("Buffer allocation failed");
        exit(1);
    }
    
    buffer->items = (int*)malloc(size * sizeof(int));
    if (!buffer->items) {
        perror("Items allocation failed");
        free(buffer);
        exit(1);
    }
    
    buffer->size = size;
    buffer->front = 0;
    buffer->rear = 0;
    buffer->count = 0;
    
    pthread_mutex_init(&buffer->mutex, NULL);
    pthread_cond_init(&buffer->can_produce, NULL);
    pthread_cond_init(&buffer->can_consume, NULL);
    
    return buffer;
}

// Function to display buffer status
void displayBuffer(CircularBuffer* buffer) {
    printf("\nBuffer Status: [");
    int current = buffer->front;
    for (int i = 0; i < buffer->size; i++) {
        if (i < buffer->count) {
            printf("%d", buffer->items[current]);
            current = (current + 1) % buffer->size;
        } else {
            printf("_");
        }
        if (i < buffer->size - 1) printf(",");
    }
    printf("] (Count: %d)\n", buffer->count);
}

// Producer thread function
void* producer(void* arg) {
    ThreadParams* params = (ThreadParams*)arg;
    CircularBuffer* buffer = params->buffer;
    int id = params->id;
    
    while (1) {
        // Generate item to produce
        int item = (id * 100) + (rand() % 100);
        
        pthread_mutex_lock(&buffer->mutex);
        
        printf("\nProducer %d: Waiting to produce...\n", id);
        while (buffer->count == buffer->size) {
            pthread_cond_wait(&buffer->can_produce, &buffer->mutex);
        }
        
        // Produce item
        buffer->items[buffer->rear] = item;
        buffer->rear = (buffer->rear + 1) % buffer->size;
        buffer->count++;
        
        printf("Producer %d: Produced item %d\n", id, item);
        displayBuffer(buffer);
        
        pthread_cond_signal(&buffer->can_consume);
        pthread_mutex_unlock(&buffer->mutex);
        
        // Random sleep between 1-3 seconds
        sleep(rand() % 3 + 1);
    }
    return NULL;
}

// Consumer thread function
void* consumer(void* arg) {
    ThreadParams* params = (ThreadParams*)arg;
    CircularBuffer* buffer = params->buffer;
    int id = params->id;
    
    while (1) {
        pthread_mutex_lock(&buffer->mutex);
        
        printf("\nConsumer %d: Waiting to consume...\n", id);
        while (buffer->count == 0) {
            pthread_cond_wait(&buffer->can_consume, &buffer->mutex);
        }
        
        // Consume item
        int item = buffer->items[buffer->front];
        buffer->front = (buffer->front + 1) % buffer->size;
        buffer->count--;
        
        printf("Consumer %d: Consumed item %d\n", id, item);
        displayBuffer(buffer);
        
        pthread_cond_signal(&buffer->can_produce);
        pthread_mutex_unlock(&buffer->mutex);
        
        // Random sleep between 2-4 seconds
        sleep(rand() % 3 + 2);
    }
    return NULL;
}

// Cleanup function
void cleanup(CircularBuffer* buffer, ThreadParams* prod_params, ThreadParams* cons_params,
            pthread_t* prod_threads, pthread_t* cons_threads) {
    pthread_mutex_destroy(&buffer->mutex);
    pthread_cond_destroy(&buffer->can_produce);
    pthread_cond_destroy(&buffer->can_consume);
    free(buffer->items);
    free(buffer);
    free(prod_params);
    free(cons_params);
    free(prod_threads);
    free(cons_threads);
}

int main() {
    int buffer_size, num_producers, num_consumers;
    
    // Get user input
    printf("Enter buffer size: ");
    scanf("%d", &buffer_size);
    printf("Enter number of producers: ");
    scanf("%d", &num_producers);
    printf("Enter number of consumers: ");
    scanf("%d", &num_consumers);
    
    // Initialize random seed
    srand(time(NULL));
    
    // Initialize buffer
    CircularBuffer* buffer = initBuffer(buffer_size);
    
    // Create thread arrays and parameters
    pthread_t* prod_threads = malloc(num_producers * sizeof(pthread_t));
    pthread_t* cons_threads = malloc(num_consumers * sizeof(pthread_t));
    ThreadParams* prod_params = malloc(num_producers * sizeof(ThreadParams));
    ThreadParams* cons_params = malloc(num_consumers * sizeof(ThreadParams));
    
    printf("\nStarting Producer-Consumer Simulation...\n");
    printf("Buffer Size: %d\n", buffer_size);
    printf("Number of Producers: %d\n", num_producers);
    printf("Number of Consumers: %d\n\n", num_consumers);
    
    // Create producer threads
    for (int i = 0; i < num_producers; i++) {
        prod_params[i].id = i + 1;
        prod_params[i].buffer = buffer;
        pthread_create(&prod_threads[i], NULL, producer, &prod_params[i]);
    }
    
    // Create consumer threads
    for (int i = 0; i < num_consumers; i++) {
        cons_params[i].id = i + 1;
        cons_params[i].buffer = buffer;
        pthread_create(&cons_threads[i], NULL, consumer, &cons_params[i]);
    }
    
    // Wait for threads
    for (int i = 0; i < num_producers; i++) {
        pthread_join(prod_threads[i], NULL);
    }
    for (int i = 0; i < num_consumers; i++) {
        pthread_join(cons_threads[i], NULL);
    }
    
    // Cleanup
    cleanup(buffer, prod_params, cons_params, prod_threads, cons_threads);
    
    return 0;
}