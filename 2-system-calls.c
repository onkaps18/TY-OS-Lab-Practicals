#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

void process_demo() {
    pid_t pid;
    int status;
    
    printf("\nProcess Related System Calls Demo:\n");
    pid = fork();
    
    if (pid < 0) {
        printf("Fork failed\n");
        exit(1);
    } else if (pid == 0) {
        printf("Child process (PID: %d)\n", getpid());
        exit(0);
    } else {
        printf("Parent process (PID: %d) waiting for child\n", getpid());
        wait(&status);
        printf("Child process completed\n");
    }
}

void file_demo() {
    int fd;
    char buffer[100];
    char *message = "Hello, this is a test message\n";
    
    printf("\nFile Related System Calls Demo:\n");
    
    // Open and write
    fd = open("test.txt", O_CREAT | O_WRONLY, 0644);
    if (fd < 0) {
        printf("Error opening file\n");
        return;
    }
    write(fd, message, strlen(message));
    close(fd);
    
    // Create hard link
    if (link("test.txt", "test_link.txt") < 0) {
        printf("Error creating link\n");
    }
    
    // Read file
    fd = open("test.txt", O_RDONLY);
    read(fd, buffer, sizeof(buffer));
    printf("File contents: %s", buffer);
    close(fd);
    
    // Get file stats
    struct stat file_stat;
    stat("test.txt", &file_stat);
    printf("File size: %ld bytes\n", file_stat.st_size);
    
    // Delete files
    unlink("test_link.txt");
    unlink("test.txt");
}

void communication_demo() {
    int pipefd[2];
    pid_t pid;
    char buffer[100];
    char *message = "Message through pipe\n";
    
    printf("\nCommunication System Calls Demo:\n");
    
    if (pipe(pipefd) < 0) {
        printf("Pipe creation failed\n");
        return;
    }
    
    pid = fork();
    
    if (pid < 0) {
        printf("Fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child process
        close(pipefd[1]);  // Close write end
        read(pipefd[0], buffer, sizeof(buffer));
        printf("Child received: %s", buffer);
        close(pipefd[0]);
        exit(0);
    } else {
        // Parent process
        close(pipefd[0]);  // Close read end
        write(pipefd[1], message, strlen(message));
        close(pipefd[1]);
        wait(NULL);
    }
    
    // FIFO demo
    if (mkfifo("myfifo", 0666) < 0) {
        printf("FIFO creation failed\n");
    } else {
        printf("FIFO created successfully (myfifo)\n");
        unlink("myfifo");  // Clean up
    }
}

void info_demo() {
    printf("\nSystem Information System Calls Demo:\n");
    
    // Process ID
    printf("Process ID: %d\n", getpid());
    
    // Parent Process ID
    printf("Parent Process ID: %d\n", getppid());
    
    // Current working directory
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    }
    
    // User ID
    printf("User ID: %d\n", getuid());
    
    // Group ID
    printf("Group ID: %d\n", getgid());
}

int main() {
    int choice;
    
    while (1) {
        printf("\nSystem Calls Demonstration Menu\n");
        printf("1. Process Related System Calls\n");
        printf("2. File Related System Calls\n");
        printf("3. Communication System Calls\n");
        printf("4. Information Related System Calls\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                process_demo();
                break;
            case 2:
                file_demo();
                break;
            case 3:
                communication_demo();
                break;
            case 4:
                info_demo();
                break;
            case 5:
                printf("Exiting program...\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    
    return 0;
}