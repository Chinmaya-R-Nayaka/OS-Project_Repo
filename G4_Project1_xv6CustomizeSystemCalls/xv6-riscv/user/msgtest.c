/* --- Bhanu's Work: Message Passing IPC --- */
/*
 * msgtest.c
 *
 * Demonstration program for the Direct Message Passing IPC system.
 *
 * Test layout:
 *   1. The parent process forks a child and learns its PID.
 *   2. The parent sends TWO messages to the child's mailbox queue.
 *   3. The child calls recv() twice to dequeue both messages in FIFO order.
 *   4. The child prints each received message and exits.
 *   5. The parent waits for the child and prints a confirmation.
 *
 * Expected output when run from the xv6 shell:
 *   [parent] My PID is X. Forked child with PID Y.
 *   [parent] Sent message 1 to child.
 *   [parent] Sent message 2 to child.
 *   [child]  Received msg 1: Hello from the parent! (message 1)
 *   [child]  Received msg 2: This is the second message in the queue.
 *   [parent] Child finished. IPC test complete.
 */

#include "kernel/types.h"   // uint, etc.
#include "kernel/stat.h"    // struct stat (required by user.h)
#include "user/user.h"      // fork, wait, send, recv, printf, exit, getpid

int
main(void)
{
  /* Buffer large enough for one MSG_SIZE (128-byte) message */
  char buf[128];

  /* -------------------------------------------------------
   * Fork a child process.
   * fork() returns 0 to the child, and the child's PID to
   * the parent (following the standard Unix convention).
   * ------------------------------------------------------- */
  int child_pid = fork();

  if(child_pid < 0){
    /* fork() failed — no free process slots */
    printf("msgtest: fork failed\n");
    exit(1);
  }

  /* ===================== CHILD PROCESS ===================== */
  if(child_pid == 0){
    /*
     * We are the child. Our own PID was assigned by the kernel.
     * We call recv() twice to dequeue the two messages the parent
     * is going to send. recv() will BLOCK here until a message
     * arrives if the queue is empty.
     */

    /* Receive first message from the queue */
    recv(buf);
    printf("[child]  Received msg 1: %s\n", buf);

    /* Receive second message from the queue */
    recv(buf);
    printf("[child]  Received msg 2: %s\n", buf);

    /* All done — exit cleanly */
    exit(0);
  }

  /* ===================== PARENT PROCESS =================== */
  /*
   * We are the parent. child_pid holds the PID of our new child.
   * We print a header so we can verify the PIDs are correct.
   */
  printf("[parent] My PID is %d. Forked child with PID %d.\n",
         getpid(), child_pid);

  /*
   * Send message 1 into the child's mailbox.
   * send() returns 0 on success, -1 if the queue is full or
   * the target PID doesn't exist.
   */
  if(send(child_pid, "Hello from the parent! (message 1)") < 0){
    printf("[parent] ERROR: failed to send message 1\n");
    exit(1);
  }
  printf("[parent] Sent message 1 to child.\n");

  /*
   * Send message 2 — demonstrates that the queue holds >1 item.
   */
  if(send(child_pid, "This is the second message in the queue.") < 0){
    printf("[parent] ERROR: failed to send message 2\n");
    exit(1);
  }
  printf("[parent] Sent message 2 to child.\n");

  /*
   * Wait for the child to finish receiving and printing both messages.
   * wait() blocks until the child calls exit().
   */
  wait(0);
  printf("[parent] Child finished. IPC test complete.\n");

  exit(0);
}
/* ----------------------------------------- */
