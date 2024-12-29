#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "directory_ops.h"

void list_directory(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;

    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    printf("Contents of directory: %s\n", path);
    printf("-------------------------------------------------\n");
    printf("Type\tSize\t\tName\n");
    printf("-------------------------------------------------\n");

    while ((entry = readdir(dir)) != NULL) {
        // Build full path for stat
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (stat(full_path, &file_stat) == -1) {
            perror("stat");
            continue;
        }

        // Print file type
        if (S_ISDIR(file_stat.st_mode))
            printf("DIR\t");
        else if (S_ISREG(file_stat.st_mode))
            printf("FILE\t");
        else
            printf("OTHER\t");

        // Print size and name
        printf("%ld\t\t%s\n", file_stat.st_size, entry->d_name);
    }

    closedir(dir);
}
