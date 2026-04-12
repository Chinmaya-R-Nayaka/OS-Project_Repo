#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    struct dirent *d;
    
    // Open current directory by default, or the directory provided as an argument
    DIR *dh = opendir(argc > 1 ? argv[1] : ".");
    
    if (!dh) {
        perror("custom_ls error");
        return 1;
    }
    
    // Reading the directory contents 
    while ((d = readdir(dh)) != NULL) {
        // Skipping hidden files (files starting with '.') like actual 'ls' behavior
        if (d->d_name[0] == '.') {
            continue; 
        }
        printf("%s  ", d->d_name);
    }
    printf("\n");
    
    // Closing the directory stream
    closedir(dh);
    return 0;
}
