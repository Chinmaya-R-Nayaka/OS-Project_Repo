#include "kernel/types.h"
#include "user/user.h"


void heavy_work() {
  for (volatile int i = 0; i < 100000000; i++) {
      // The 'volatile' keyword prevents the compiler 
  }
}
int main() {
  int pid;

  for (int i = 1; i <= 3; i++) {
    pid = fork();
    if (pid == 0) { // Child
      int pri = (4-i) * 3; // Priorities: 9, 6, 3
      setpriority(getpid(), pri);
      printf("Child %d started with priority %d\n", getpid(), pri);
      
       heavy_work(); // Just to waste time
      
      printf("Child %d (Priority %d) FINISHED\n", getpid(), pri);
      exit(0);
    }
  }

  for (int i = 0; i < 3; i++) wait(0);
  printf("Test complete.\n");
  exit(0);
}
