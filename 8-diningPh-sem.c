#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define THINKING 0
#define HUNGRY 1
#define EATING 2

// Structure to hold philosopher information
struct Philosopher {
    int id;
    int state;
    pthread_t thread;
};

// Global variables
struct Philosopher *philosophers;
sem_t *forks;
sem_t mutex;
int num_philosophers;

void print_states() {
    printf("\nPhilosophers' current states:\n");
    for (int i = 0; i < num_philosophers; i++) {
        printf("Philosopher %d: ", i + 1);
        switch (philosophers[i].state) {
            case THINKING: printf("Thinking "); break;
            case HUNGRY: printf("Hungry "); break;
            case EATING: printf("Eating "); break;
        }
        printf("\n");
    }
    printf("\n");
}

void test(int phil_id) {
    if (philosophers[phil_id].state == HUNGRY &&
        philosophers[(phil_id + num_philosophers - 1) % num_philosophers].state != EATING &&
        philosophers[(phil_id + 1) % num_philosophers].state != EATING) {
        
        philosophers[phil_id].state = EATING;
        sem_post(&forks[phil_id]);
    }
}

void take_forks(int phil_id) {
    sem_wait(&mutex);
    philosophers[phil_id].state = HUNGRY;
    print_states();
    test(phil_id);
    sem_post(&mutex);
    sem_wait(&forks[phil_id]);
}

void put_forks(int phil_id) {
    sem_wait(&mutex);
    philosophers[phil_id].state = THINKING;
    print_states();
    
    // Test left and right neighbors
    test((phil_id + num_philosophers - 1) % num_philosophers);
    test((phil_id + 1) % num_philosophers);
    
    sem_post(&mutex);
}

void* philosopher(void* arg) {
    int phil_id = *(int*)arg;
    int think_time, eat_time;
    
    while (1) {
        // Thinking
        think_time = (rand() % 3) + 1;
        printf("\nPhilosopher %d is thinking for %d seconds...\n", phil_id + 1, think_time);
        sleep(think_time);
        
        // Pick up forks
        take_forks(phil_id);
        
        // Eating
        eat_time = (rand() % 3) + 1;
        printf("\nPhilosopher %d is eating for %d seconds...\n", phil_id + 1, eat_time);
        sleep(eat_time);
        
        // Put down forks
        put_forks(phil_id);
    }
    return NULL;
}

int main() {
    printf("Enter the number of philosophers (3-10): ");
    scanf("%d", &num_philosophers);
    
    if (num_philosophers < 3 || num_philosophers > 10) {
        printf("Invalid number of philosophers. Please enter a number between 3 and 10.\n");
        return 1;
    }
    
    // Initialize random seed
    srand(time(NULL));
    
    // Allocate memory for philosophers and forks
    philosophers = (struct Philosopher*)malloc(num_philosophers * sizeof(struct Philosopher));
    forks = (sem_t*)malloc(num_philosophers * sizeof(sem_t));
    
    // Initialize mutex and forks
    sem_init(&mutex, 0, 1);
    for (int i = 0; i < num_philosophers; i++) {
        sem_init(&forks[i], 0, 0);
    }
    
    // Initialize philosophers
    for (int i = 0; i < num_philosophers; i++) {
        philosophers[i].id = i;
        philosophers[i].state = THINKING;
        int* arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&philosophers[i].thread, NULL, philosopher, arg);
    }
    
    printf("\n");
    
    // Wait for all philosopher threads
    for (int i = 0; i < num_philosophers; i++) {
        pthread_join(philosophers[i].thread, NULL);
    }
    
    // Cleanup
    sem_destroy(&mutex);
    for (int i = 0; i < num_philosophers; i++) {
        sem_destroy(&forks[i]);
    }
    free(philosophers);
    free(forks);
    
    return 0;
}