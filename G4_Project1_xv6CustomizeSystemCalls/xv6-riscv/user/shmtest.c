#include "kernel/types.h"
#include "user/user.h"

int main() {
    int shmid = shmget();
    int *ptr = (int*) shmat(shmid);

    *ptr = 100;

    int pid = fork();

    if(pid == 0) {
        // Child
        printf("Child sees: %d\n", *ptr);
        *ptr = 200;
        printf("Child updated to: %d\n", *ptr);
    } else {
        wait(0);
        printf("Parent sees: %d\n", *ptr);
    }

    exit(0);
}
