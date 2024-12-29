#include <stdio.h>
#include <string.h>
#include "directory_ops.h"
#include "file_operations.h"
#include "logger.h"

void print_help(); // Forward declaration for help

int main() {
    char command[256];

    printf("Welcome to the Terminal File Manager!\n");
    printf("Type 'help' to see available commands.\n");

    while (1) {
        printf("> ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            perror("fgets");
            return 1;
        }

        // Remove trailing newline character
        command[strcspn(command, "\n")] = 0;

        // Command parsing and logging
        if (strncmp(command, "slist ", 6) == 0) {
            char *path = command + 6; // Extract the path after 'slist '
            if (*path == '\0') {
                printf("Error: Please provide a valid directory path.\n");
                log_operation("slist", path, "Error: Invalid path");
            } else {
                list_directory(path); // Call the list_directory function
                log_operation("slist", path, "Success");
            }
        } else if (strncmp(command, "sremove ", 8) == 0) {
            char *path = command + 8; // Extract the path after 'sremove '
            if (*path == '\0') {
                printf("Error: Please provide a valid path to remove.\n");
                log_operation("sremove", path, "Error: Invalid path");
            } else {
                sremove(path); // Remove file or directory
                log_operation("sremove", path, "Success");
            }
        } else if (strncmp(command, "scopy ", 6) == 0) {
            char *src_path = command + 6; // Extract the source path
            char *dest_path = strchr(src_path, ' '); // Find the space separating paths

            if (dest_path) {
                *dest_path = '\0'; // Null-terminate the source path
                dest_path++; // Move past the space to the destination path
                if (*dest_path == '\0') {
                    printf("Error: Destination path is missing.\n");
                    log_operation("scopy", src_path, "Error: Missing destination path");
                } else {
                    scopy(src_path, dest_path); // Call the scopy function
                    log_operation("scopy", src_path, "Success");
                }
            } else {
                printf("Usage: scopy <source> <destination>\n");
                log_operation("scopy", src_path, "Error: Invalid arguments");
            }
        } else if (strncmp(command, "sfile ", 6) == 0) {
            char *path = command + 6;
            sfile(path);  // Call the sfile function to display file content
            log_operation("sfile", path, "Success");
        } else if (strncmp(command, "screate ", 8) == 0) {
            char *path = command + 8;  // Extract the path after 'screate '
            screate(path);             // Call the screate function to create the directory
            log_operation("screate", path, "Success");
        } else if (strncmp(command, "ssearch ", 8) == 0) {
            char *dir_path = command + 8;
            char *filename = strchr(dir_path, ' ');
            if (filename) {
                *filename = '\0';
                filename++;
                ssearch(dir_path, filename);  // Call the search function
                log_operation("ssearch", dir_path, "Success");
            } else {
                printf("Usage: ssearch <directory_path> <filename>\n");
                log_operation("ssearch", dir_path, "Error: Invalid arguments");
            }
        } else if (strncmp(command, "screatefile ", 12) == 0) {
    char *file_path = command + 12; // Extract the file path after "screatefile "
    if (*file_path == '\0') {
        printf("Error: Please provide a valid file path.\n");
    } else {
        screate_file(file_path); // Call the function to create the file
    }
}
else if (strncmp(command, "schmod ", 7) == 0) {
            char *path = command + 7;
            char *perm_str = strchr(path, ' ');
            if (perm_str) {
                *perm_str = '\0';
                perm_str++;  // Move past the space to the permission string
                schmod(path, perm_str);  // Call the schmod function
                log_operation("schmod", path, "Success");
            } else {
                printf("Usage: schmod <path> <permissions>\n");
                log_operation("schmod", path, "Error: Invalid arguments");
            }
        } else if (strncmp(command, "sremovedir ", 11) == 0) {
            char *path = command + 11;  // Extract the path after 'sremovedir '
            sremove_dir(path);          // Call the sremove_dir function to remove the directory
            log_operation("sremovedir", path, "Success");
        } else if (strncmp(command, "srename ", 8) == 0) {
            char *old_path = command + 8;
            char *new_path = strchr(old_path, ' ');
            if (new_path) {
                *new_path = '\0'; // Null-terminate the old path
                new_path++; // Move past the space to the new path
                srename(old_path, new_path);  // Call the srename function
                log_operation("srename", old_path, "Success");
            } else {
                printf("Usage: srename <old_path> <new_path>\n");
                log_operation("srename", old_path, "Error: Invalid arguments");
            }
        } else if (strcmp(command, "help") == 0) {
            print_help();
            log_operation("help", "N/A", "Success");
        } else if (strcmp(command, "exit") == 0) {
            printf("Exiting the program. Goodbye!\n");
            log_operation("exit", "N/A", "Success");
            break;
        } else {
            printf("Unknown command: %s\n", command);
            log_operation("unknown", command, "Error: Command not found");
        }
    }
    return 0;
}

void print_help() {
    printf("Available commands:\n");
    printf("  help                                 - Display this help message\n");
    printf("  slist <path>                         - List the contents of a directory\n");
    printf("  sremove <path>                       - Remove a file or directory\n");
    printf("  scopy <source> <destination>         - Copy a file from source to destination\n");
    printf("  sfile <path>                         - Show the content of a file\n");
    printf("  screate <path>                       - Create a directory at the specified path\n");
    printf("  ssearch <directory> <filename>       - Search for a file by name within a directory\n");
    printf("  schmod <path> <permissions>          - Modify a file or directory's permissions (e.g., rwxr-xr-x)\n");
    printf("  sremovedir <path>                    - Delete a directory that is empty\n");
    printf("  srename <old_path> <new_path>        - Rename a directory or file\n");
    printf("  screatefile <path>                   - Create a directory at the specified path\n");
    printf("  exit                                 - Exit the program\n");
}


