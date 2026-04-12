#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int pid1, pid2;
    int status;

    printf("waitpid test starting...\n");

    // Create first child
    pid1 = fork();
    if(pid1 == 0) {
        // Child 1: Shorter busy loop delay
        for(volatile int i = 0; i < 50000000; i++); 
        printf("Child 1 (PID: %d) exiting.\n", getpid());
        exit(0);
    }

    // Create second child
    pid2 = fork();
    if(pid2 == 0) {
        // Child 2: Longer busy loop delay to ensure it exits later
        for(volatile int i = 0; i < 150000000; i++); 
        printf("Child 2 (PID: %d) exiting.\n", getpid());
        exit(0);
    }

    // The Parent does the initial printing to avoid collisions
    printf("Child 1 (PID: %d) is running...\n", pid1);
    printf("Child 2 (PID: %d) is running...\n", pid2);
    
    // Test 1: Wait specifically for Child 2 (using 3 parameters)
    printf("Parent waiting specifically for Child 2 (PID: %d) first...\n", pid2);
    int ret = waitpid(pid2, &status, 0);
    printf("waitpid returned: %d (Successfully waited for Child 2)\n", ret);

    // Test 2: Wait for ANY remaining child using pid = -1 (Child 1 will be caught here)
    printf("Parent now waiting for ANY child (using pid = -1)...\n");
    ret = waitpid(-1, &status, 0);
    printf("waitpid returned: %d (Successfully waited for Child 1)\n", ret);

    printf("waitpid test complete.\n");
    exit(0);
}