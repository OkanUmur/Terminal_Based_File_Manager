#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include "file_operations.h"
#include <fcntl.h>    // For open, O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC
#include <unistd.h>   // For close
#include <time.h>  // For ctime
#include <stdlib.h>
#include <errno.h>
#include "logger.h"
#include <sys/types.h>


void screate(const char *path) {
    // Create a directory with 0755 permissions (rw-r-xr-x)
    if (mkdir(path, 0755) == -1) {
        perror("Failed to create directory");
        log_operation("screate", path, "Failed");
    } else {
        printf("Directory created: %s\n", path);
        log_operation("screate", path, "Success");
    }
}

int is_directory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) == 0) {
        return S_ISDIR(statbuf.st_mode);  // Return true if it's a directory
    }
    return 0;  // False if it's not a directory or doesn't exist
}

void sremove(const char *path) {
    if (is_directory(path)) {
        // Check if the directory is empty
        DIR *dir = opendir(path);
        if (dir) {
            struct dirent *entry;
            int is_empty = 1;

            // Check if directory is empty
            while ((entry = readdir(dir)) != NULL) {
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                    is_empty = 0;
                    break;
                }
            }
            closedir(dir);

            if (is_empty) {
                // Remove empty directory
                if (rmdir(path) == 0) {
                    printf("Directory removed successfully: %s\n", path);
                } else {
                    perror("Failed to remove directory");
                }
            } else {
                printf("Directory is not empty, cannot remove: %s\n", path);
            }
        } else {
            perror("Failed to open directory");
        }
    } else {
        // Remove file using unlink
        if (unlink(path) == 0) {
            printf("File removed successfully: %s\n", path);
        } else {
            perror("Failed to remove file");
        }
    }
}

void scopy(const char *src_path, const char *dest_path) {

    printf("Source Path: %s\n", src_path);  // Debugging output
    printf("Destination Path: %s\n", dest_path);  // Debugging output

    // The rest of the file copy logic...


    int src_fd = open(src_path, O_RDONLY);
    if (src_fd == -1) {
        perror("Failed to open source file");
        return;  // Exit the function if file open fails
    }

    int dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (dest_fd == -1) {
        perror("Failed to open destination file");
        close(src_fd);
        return;  // Exit the function if file open fails
    }

    char buffer[1024];
    ssize_t bytes_read, bytes_written;

    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Write error");
            break;
        }
    }

    if (bytes_read == -1) {
        perror("Read error");
    }

    close(src_fd);
    close(dest_fd);
    printf("File copied successfully.\n");
}

void sfile(const char *path) {
    int fd;
    char buffer[1024];
    ssize_t bytes_read;

    // Open the file
    fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }
 struct stat file_info;
    if (stat(path, &file_info) == -1) {
        perror("Failed to get file information");
        return;
    }

    printf("File: %s\n", path);
    printf("Size: %ld bytes\n", file_info.st_size);
    printf("Permissions: %o\n", file_info.st_mode & 0777);  // Print permissions in octal
    printf("Last modified: %s", ctime(&file_info.st_mtime));

    // Read the file and display the content
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);  // Print to standard output
    }

    if (bytes_read == -1) {
        perror("Error reading file");
    }

    // Close the file
    close(fd);
}


void ssearch(const char *dir_path, const char *filename) {
    DIR *dir = opendir(dir_path);  // Open the directory
    if (dir == NULL) {
        perror("Failed to open directory");
        return;
    }

    struct dirent *entry;
    int found = 0;

    // Iterate over each entry in the directory
    while ((entry = readdir(dir)) != NULL) {
        // Use strstr to check if the filename contains the search term
        if (strstr(entry->d_name, filename) != NULL) {
            printf("Found: %s/%s\n", dir_path, entry->d_name);  // Print matching file name
            found = 1;
        }
    }

    if (!found) {
        printf("No file named '%s' found in directory '%s'.\n", filename, dir_path);
    }

    closedir(dir);  // Close the directory
}

void schmod(const char *path, const char *perm_str) {
    // Validate the permission string
    if (strlen(perm_str) != 9) {
        fprintf(stderr, "Invalid permission string: %s. It must be 9 characters long (e.g., rwxr-xr-x).\n", perm_str);
        return;
    }

    // Convert permission string to octal format
    mode_t new_mode = 0;

    // Owner's permissions (first 3 characters)
    if (perm_str[0] == 'r') new_mode |= S_IRUSR;
    if (perm_str[1] == 'w') new_mode |= S_IWUSR;
    if (perm_str[2] == 'x') new_mode |= S_IXUSR;

    // Group's permissions (next 3 characters)
    if (perm_str[3] == 'r') new_mode |= S_IRGRP;
    if (perm_str[4] == 'w') new_mode |= S_IWGRP;
    if (perm_str[5] == 'x') new_mode |= S_IXGRP;

    // Others' permissions (last 3 characters)
    if (perm_str[6] == 'r') new_mode |= S_IROTH;
    if (perm_str[7] == 'w') new_mode |= S_IWOTH;
    if (perm_str[8] == 'x') new_mode |= S_IXOTH;

    // Apply the permissions using chmod
    if (chmod(path, new_mode) == -1) {
        perror("Failed to change file permissions");
    } else {
        printf("Permissions for '%s' changed successfully.\n", path);
    }
}

void sremove_dir(const char *path) {
    // Attempt to remove the directory
    if (rmdir(path) == -1) {
        perror("Failed to remove directory");
        log_operation("sremove_dir", path, "Failed");
    } else {
        printf("Directory removed: %s\n", path);
        log_operation("sremove_dir", path, "Success");
    }
}

void srename(const char *old_path, const char *new_path) {
    // Attempt to rename the file or directory
    if (rename(old_path, new_path) == -1) {
        perror("Failed to rename");
        log_operation("srename", old_path, "Failed");
    } else {
        printf("Successfully renamed/moved: %s to %s\n", old_path, new_path);
        log_operation("srename", old_path, "Success");
    }
}

void screate_file(const char *file_path) {
    // Use open system call with O_CREAT and O_WRONLY flags to create the file
    int fd = open(file_path, O_CREAT | O_WRONLY, 0644); // 0644 is the permission
    if (fd == -1) {
        // Log or print the error if file creation fails
        printf("Failed to create file '%s': %s\n", file_path, strerror(errno));
    } else {
        printf("File '%s' created successfully.\n", file_path);
        close(fd); // Close the file descriptor
    }
}

void write_to_file(const char *file_path, const char *content) {
    int fd = open(file_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        printf("Failed to open or create file '%s' for writing: %s\n", file_path, strerror(errno));
        return;
    }

    ssize_t bytes_written = write(fd, content, strlen(content));
    if (bytes_written == -1) {
        printf("Failed to write to file '%s': %s\n", file_path, strerror(errno));
    }

    if (close(fd) == -1) {
        printf("Failed to close file '%s': %s\n", file_path, strerror(errno));
    }
}

void read_from_file(const char *file_path) {
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        printf("Failed to open file '%s' for reading: %s\n", file_path, strerror(errno));
        return;
    }

    char buffer[1024];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        printf("Failed to read from file '%s': %s\n", file_path, strerror(errno));
    } else if (bytes_read == 0) {
        printf("End of file reached.\n");
    } else {
        buffer[bytes_read] = '\0'; // Null-terminate the buffer
        printf("File Content:\n%s\n", buffer);
    }

    if (close(fd) == -1) {
        printf("Failed to close file '%s': %s\n", file_path, strerror(errno));
    }
}
