#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <libgen.h> // For the basename() function

int main(int argc,char *argv[]) {
    // 1. Check for correct number of arguments
    if (argc!=3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    char *src=argv[1];
    char *dest=argv[2];
    struct stat dest_stat;

    // 2. Check if the destination is an existing directory
    if(stat(dest,&dest_stat)==0 && S_ISDIR(dest_stat.st_mode)) {
        // If destination is a folder, construct the new path: dest_folder/filename
        char *base=basename(src); 
        char new_dest[1024];
        
        // Safely format the new path string
        snprintf(new_dest,sizeof(new_dest),"%s/%s",dest,base);

        // Move the file into the directory
        if(rename(src,new_dest) != 0) {
            perror("custom_mv error");
            return 1;
        }
    } 
    else {
        // 3. Destination is either a file name or doesn't exist, so we just rename
        if(rename(src,dest)!=0) {
            perror("custom_mv error");
            return 1;
        }
    }

    return 0;
}