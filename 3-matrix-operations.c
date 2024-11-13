#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_SIZE 1000

int matA[MAX_SIZE][MAX_SIZE];
int matB[MAX_SIZE][MAX_SIZE];
int res[MAX_SIZE][MAX_SIZE];
int matrix_size;

struct thread_data {
    int row;
    char op;
};

void input_matrix (int matrix[MAX_SIZE][MAX_SIZE],const char* name) {
    printf("Enter the elements for matrix %s: \n", name);
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            printf("%s[%d][%d]: ", name, i, j);
            scanf("%d", &matrix[i][j]);
        }
    }
}

void *perform_operation (void *arg) {
    struct thread_data *data = (struct thread_data *)arg;
    int i, j, k;

    switch (data->op) {
        case 'M':
            for (j = 0; j < matrix_size; j++) {
                res[data->row][j] = 0;
                for (k = 0; k < matrix_size; k++) {
                    res[data->row][j] += matA[data->row][k] * matB[k][j];
                }
            }
            break;

        case 'A':
            for (j = 0; j < matrix_size; j++) {
                res[data->row][j] = matA[data->row][j] + matB[data->row][j];
            }
            break;

        case 'S': 
            for (j = 0; j < matrix_size; j++) {
                res[data->row][j] = matA[data->row][j] - matB[data->row][j];
            }
            break;  
    }
    pthread_exit(NULL);
}

void print_matrix (int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void perform_threaded_operation(char op) {
    pthread_t threads[MAX_SIZE];
    struct thread_data thread_data_array[MAX_SIZE];
    int rc, t;

    for (t = 0; t < matrix_size; t++) {
        thread_data_array[t].row = t;
        thread_data_array[t].op = op;

        rc = pthread_create(&threads[t], NULL, perform_operation, (void *)&thread_data_array[t]);
        if (rc) {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for (t = 0; t < matrix_size; t++) {
        pthread_join(threads[t], NULL);
    }

}


int main() {
    char operation;

    printf("Enter the size of the matrix: ");
    scanf("%d", &matrix_size);

    if (matrix_size > MAX_SIZE || matrix_size <= 0) {
        printf("Invalid matrix size. Please enter a size between 1 and %d.\n", MAX_SIZE);
    }

    input_matrix(matA, "A");
    input_matrix(matB, "B");

    while (1)
    {
        printf("\nChoose Operation: \n");
        printf("M - multiplication\n");
        printf("A - addition\n");
        printf("S - subtraction\n");
        printf("Q - quit\n");
        printf("Enter Choice: ");
        scanf(" %c", &operation);

        if (operation == 'Q' || operation == 'q') {
            break;
        }

        if (operation != 'M' && operation != 'm' && operation != 'A' && operation != 'a' && operation != 'S' && operation != 's') {
            printf("Invalid operation. Please enter a valid operation.\n");
            continue;
        }

        perform_threaded_operation(operation);
        printf("Result matrix: \n");
        print_matrix(res);



    }
    


    

    return 0;
}