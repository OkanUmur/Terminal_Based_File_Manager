#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H


void scopy(const char *src_path, const char *dest_path);   // Copy file
void sremove(const char *path);                            // Remove file
void sfile(const char *path);                              // Display file content
void ssearch(const char *dir_path, const char *filename);  // Search for a file by name
void schmod(const char *path, const char *perm_str);
void screate(const char *path);  // Function for creating a directory
void sremove_dir(const char *path);  // Function for removing a directory
void screate_file(const char *file_path);
void srename(const char *old_path, const char *new_path);  // Function to rename a file or folder



// Function declarations for file operations go here

#endif
