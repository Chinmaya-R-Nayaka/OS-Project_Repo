#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fnmatch.h>

// Recursive function to search through directories (Optimized with Turbo Search - d_type)
void search_dir(const char *dir_name, const char *search_name, int flags, int recursive) {
    DIR *d;
    struct dirent *dir;
    d = opendir(dir_name);
    
    if (d == NULL) return;
    
    while ((dir = readdir(d)) != NULL) {
        // Skip . and ..
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
            continue;
        }
        
        char path[4096];
        int len = strlen(dir_name);
        if (len > 0 && (dir_name[len - 1] == '/' || dir_name[len - 1] == '\\')) {
            snprintf(path, sizeof(path), "%s%s", dir_name, dir->d_name);
        } else {
            snprintf(path, sizeof(path), "%s/%s", dir_name, dir->d_name);
        }
        
        // Print if it matches search criteria
        if (search_name == NULL) {
            printf("%s\n", path);
        } else if (fnmatch(search_name, dir->d_name, flags) == 0) {
            printf("MATCH: %s\n", path);
        }
        
        // TURBO RECURSION LOGIC
        if (recursive) {
            int is_dir = 0;
            
            // Look at the d_type "Tag" - this is lightning fast!
            if (dir->d_type == DT_DIR) {
                is_dir = 1;
            } 
            // Fallback: only if the system doesn't know the type, then we use slow stat()
            else if (dir->d_type == DT_UNKNOWN) {
                struct stat stat_buf;
                if (stat(path, &stat_buf) == 0 && S_ISDIR(stat_buf.st_mode)) {
                    is_dir = 1;
                }
            }
            
            // Go deeper if it's a folder
            if (is_dir) {
                search_dir(path, search_name, flags, recursive);
            }
        }
    }
    closedir(d);
}

int main(int argc, char *argv[]) {
    const char *start_path = ".";
    const char *search_name = NULL;
    int flags = 0;
    int recursive = 0;
    
    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            recursive = 1;
        } else if (strcmp(argv[i], "-name") == 0 || strcmp(argv[i], "-iname") == 0) {
            if (i + 1 < argc) {
                if (strcmp(argv[i], "-iname") == 0) flags |= FNM_CASEFOLD;
                search_name = argv[i + 1];
                i++;
            } else {
                fprintf(stderr, "find: missing argument to `%s'\n", argv[i]);
                return 1;
            }
        } else if (argv[i][0] != '-') {
            start_path = argv[i];
        } else {
            fprintf(stderr, "find: unknown flag `%s'\n", argv[i]);
            return 1;
        }
    }
    
    // Check root folder itself
    if (search_name == NULL) {
        printf("%s\n", start_path);
    } else {
        if (fnmatch(search_name, start_path, flags) == 0) {
             printf("MATCH: %s\n", start_path);
        }
    }
    
    // Begin scanning
    search_dir(start_path, search_name, flags, recursive);
    
    return 0;
}
