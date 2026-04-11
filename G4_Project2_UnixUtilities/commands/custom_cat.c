#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    // Check arguments
    if(argc < 2){
        write(2, "Usage: custom_cat <file>\n", 25);
        return 1;
    }

    // Open file
    int fd = open(argv[1], O_RDONLY);
    if(fd < 0){
        perror("Error opening file");
        return 1;
    }

    char buffer[1024];
    int bytes_read;

    // Read and print loop
    while((bytes_read = read(fd, buffer, sizeof(buffer))) > 0){
        write(1, buffer, bytes_read);  // 1 = stdout
    }

    //  Read error check
    if(bytes_read < 0){
        perror("Error reading file");
    }

    // Close file
    close(fd);
    return 0;
}
