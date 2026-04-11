#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int pid = getpid();
    printf("Starting priority test...\n");
    
    // Change my own priority to maximum (10)
    if(setpriority(pid, 10) == 0) {
        printf("Successfully set priority of process %d to 10!\n", pid);
    } else {
        printf("Failed to set priority.\n");
    }
    
    exit(0);
}
