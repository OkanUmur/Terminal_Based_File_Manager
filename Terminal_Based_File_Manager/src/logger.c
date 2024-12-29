#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

void log_system_call(const char *call_name, const char *path, int result) {
    int log_fd = open("/home/okan-umur/Desktop/Terminal_Based_File_Manager/src/log_file.log", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    if (log_fd == -1) {
        perror("Failed to open log file");
        return;
    }

    // Get the current time
    time_t now = time(NULL);
    char *timestamp = ctime(&now);
    timestamp[strcspn(timestamp, "\n")] = '\0';  // Remove the newline character

    // Prepare the log message for system call
    char log_message[512];
    snprintf(log_message, sizeof(log_message), "[%s] System Call: %s, Path: %s, Result: %s\n", 
             timestamp, call_name, path, result == 0 ? "Success" : "Failure");

    // Write the log message to the log file
    ssize_t bytes_written = write(log_fd, log_message, strlen(log_message));
    if (bytes_written == -1) {
        perror("Failed to write to log file");
    }

    close(log_fd);  // Close the log file
}

void log_operation(const char *operation, const char *path, const char *status) {
    int log_fd = open("/home/okan-umur/Desktop/Terminal_Based_File_Manager/src/log_file.log", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    if (log_fd == -1) {
        perror("Failed to open log file");
        return;
    }

    // Get the current time
    time_t now = time(NULL);
    char *timestamp = ctime(&now);
    timestamp[strcspn(timestamp, "\n")] = '\0';  // Remove the newline character

    // Prepare the log message
    char log_message[512];
    snprintf(log_message, sizeof(log_message), "[%s] Operation: %s, Path: %s, Status: %s\n", 
             timestamp, operation, path, status);

    // Write the log message to the log file
    ssize_t bytes_written = write(log_fd, log_message, strlen(log_message));
    if (bytes_written == -1) {
        perror("Failed to write to log file");
    }

    close(log_fd);  // Close the log file
}

