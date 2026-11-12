#include <stdio.h>              // Standard I/O for printf, fgets
#include <stdlib.h>             // Standard library for exit, EXIT_FAILURE
#include <unistd.h>             // UNIX system calls like fork, pipe, read, write, close
#include <string.h>             // String functions like strlen, strtok
#include <fcntl.h>              // File control options for open()
#include <sys/types.h>          // Defines data types like pid_t
#include <sys/wait.h>           // For wait() to wait on child processes

// Function to log messages to a file with error handling
void log_message(const char *filename, const char *message) 
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644); // Open log file in write-only, append mode, create if not exist
    if (fd < 0) 
    {
        perror("\n\n\n\t\t\t\tERROR OPENING LOG FILE"); // Print error if opening failed
        exit(EXIT_FAILURE);                              // Exit if file open failed
    }

    // Write message and newline to file, check for errors
    if (write(fd, message, strlen(message)) < 0 || write(fd, "\n", 1) < 0) 
    {
        perror("\n\n\n\t\t\t\tERROR WRITING TO LOG FILE"); // Print error if writing failed
        close(fd);                                         // Close file descriptor
        exit(EXIT_FAILURE);                                // Exit on failure
    }

    close(fd); // Close the file after writing
}



int main() 
{
    int pipeAB[2], pipeBC[2], pipeCD[2], pipeDA[2];   // Declare four pipes for communication between processes
    pid_t pid_B, pid_C, pid_D;                        // Process IDs for child processes B, C, and D
    char message[256];                                // Buffer to hold the message

    // Create the unnamed pipes and check if creation fails
    if (pipe(pipeAB) == -1 || pipe(pipeBC) == -1 || pipe(pipeCD) == -1 || pipe(pipeDA) == -1) 
    {
        perror("\n\n\n\t\t\t\t---> PIPE CREATION FAILED"); // Error if pipe creation fails
        exit(EXIT_FAILURE);                                // Exit on pipe creation failure
    }

    // Fork Process B
    if ((pid_B = fork()) == 0) 
    {
        close(pipeAB[1]);         // Close unused write end of pipeAB
        close(pipeBC[0]);         // Close unused read end of pipeBC
        close(pipeCD[0]); close(pipeCD[1]);   // Close pipeCD completely (not used)
        close(pipeDA[0]); close(pipeDA[1]);   // Close pipeDA completely (not used)

        // Read message from Process A through pipeAB
        if (read(pipeAB[0], message, 256) < 0) 
        {
            perror("\n\n\n\t\t\t\t---> PROCESS B READ ERROR"); // Error in reading
            exit(EXIT_FAILURE);                                 // Exit if read fails
        }
        close(pipeAB[0]); // Close read end after reading

        // Prompt user in Process B to modify the message
        printf("\n\n\n\t\t\t\tPROCESS B, ENTER THE MODIFICATION:			");
        fgets(message + strlen(message), 256 - strlen(message), stdin); // Append modification to the message
        strtok(message, "\n"); // Remove newline character from input

        log_message("log_B.txt", message); // Log the modified message to log_B.txt

        // Send the modified message to Process C through pipeBC
        if (write(pipeBC[1], message, strlen(message) + 1) < 0) 
        {
            perror("\n\n\n\t\t\t\t---> PROCESS B WRITE ERROR"); // Error in writing
            exit(EXIT_FAILURE);                                // Exit on write failure
        }
        close(pipeBC[1]); // Close write end after sending

        exit(EXIT_SUCCESS); // Exit process B
    }

    // Fork Process C
    if ((pid_C = fork()) == 0) 
    {
        close(pipeBC[1]);         // Close unused write end of pipeBC
        close(pipeCD[0]);         // Close unused read end of pipeCD
        close(pipeAB[0]); close(pipeAB[1]);   // Close pipeAB completely
        close(pipeDA[0]); close(pipeDA[1]);   // Close pipeDA completely

        // Read message from Process B through pipeBC
        if (read(pipeBC[0], message, 256) < 0) 
        {
            perror("\n\n\n\t\t\t\t---> PROCESS C READ ERROR"); // Error in reading
            exit(EXIT_FAILURE);                                // Exit on read failure
        }
        close(pipeBC[0]); // Close read end after reading

        // Prompt user in Process C to modify the message
        printf("\n\n\n\t\t\t\tPROCESS C, ENTER MODIFICATION:				");
        fgets(message + strlen(message), 256 - strlen(message), stdin); // Append modification
        strtok(message, "\n"); // Remove newline character

        log_message("log_C.txt", message); // Log the message in log_C.txt

        // Send the modified message to Process D through pipeCD
        if (write(pipeCD[1], message, strlen(message) + 1) < 0) 
        {
            perror("\n\n\n\t\t\t\t---> PROCESS C WRITE ERROR"); // Error in writing
            exit(EXIT_FAILURE);                                // Exit on failure
        }
        close(pipeCD[1]); // Close write end after sending

        exit(EXIT_SUCCESS); // Exit process C
    }

    // Fork Process D
    if ((pid_D = fork()) == 0) 
    {
        close(pipeCD[1]);         // Close unused write end of pipeCD
        close(pipeDA[0]);         // Close unused read end of pipeDA
        close(pipeAB[0]); close(pipeAB[1]);   // Close pipeAB completely
        close(pipeBC[0]); close(pipeBC[1]);   // Close pipeBC completely

        // Read message from Process C through pipeCD
        if (read(pipeCD[0], message, 256) < 0) 
        {
            perror("\n\n\n\t\t\t\t---> PROCESS D READ ERROR"); // Error in reading
            exit(EXIT_FAILURE);                                // Exit on failure
        }
        close(pipeCD[0]); // Close read end after reading

        // Prompt user in Process D to modify the message
        printf("\n\n\n\t\t\t\tPROCESS D, ENTER MODIFICATION:				");
        fgets(message + strlen(message), 256 - strlen(message), stdin); // Append modification
        strtok(message, "\n"); // Remove newline character

        log_message("log_D.txt", message); // Log the message in log_D.txt

        // Send the final message back to Process A through pipeDA
        if (write(pipeDA[1], message, strlen(message) + 1) < 0) 
        {
            perror("\n\n\n\t\t\t\t---> PROCESS D WRITE ERROR"); // Error in writing
            exit(EXIT_FAILURE);                                // Exit on failure
        }
        close(pipeDA[1]); // Close write end after sending

        exit(EXIT_SUCCESS); // Exit process D
    }

    // Main Process A (Parent)
    close(pipeAB[0]);     // Close unused read end of pipeAB
    close(pipeDA[1]);     // Close unused write end of pipeDA
    close(pipeBC[0]); close(pipeBC[1]);   // Close pipeBC completely (handled by child)
    close(pipeCD[0]); close(pipeCD[1]);   // Close pipeCD completely (handled by child)

    // Prompt user in Process A to enter the initial message
    printf("\n\n\n\t\t\t\tPROCESS A, ENTER INITIAL MESSAGE:			");
    fgets(message, 256, stdin); // Read initial message from user
    strtok(message, "\n");           // Remove newline character

    log_message("log_A.txt", message); // Log the initial message in log_A.txt

    // Send the message to Process B through pipeAB
    if (write(pipeAB[1], message, strlen(message) + 1) < 0) 
    {
        perror("\n\n\n\t\t\t\t---> PROCESS A WRITE ERROR"); // Error in writing
        exit(EXIT_FAILURE);                                // Exit on failure
    }
    close(pipeAB[1]); // Close write end after sending

    // Read the final modified message from Process D via pipeDA
    if (read(pipeDA[0], message, 256) < 0) 
    {
        perror("\n\n\n\t\t\t\t---> PROCESS A READ ERROR"); // Error in reading
        exit(EXIT_FAILURE);                                // Exit on failure
    }
    close(pipeDA[0]); // Close read end after receiving

    // Print the final message after modifications by B, C, and D
    printf("\n\n\n\t\t\t\tFINAL MODIFIED MESSAGE RECEIVED BY PROCESS A: %s\n\n\n", message);

    // Wait for all child processes (B, C, and D) to finish
    wait(NULL);
    wait(NULL);
    wait(NULL);

    return 0; // Exit main process
}

