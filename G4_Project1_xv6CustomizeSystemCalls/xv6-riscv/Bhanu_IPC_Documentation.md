# Project: Direct Message Passing IPC in xv6

**Author:** Bhanuprakash (Group 4)

## Overview
This project adds a "mailbox" feature to xv6. It allows any running process to send and receive text messages to other processes using their Process ID (PID). Each process mailbox can hold up to **10 messages** at a time.

## Files Changed
*   **`kernel/proc.h` & `kernel/proc.c`**: Added a message queue array, queue-tracking variables (head, tail, count), and a mailbox lock (`msglock`) to the main `struct proc`.
*   **`kernel/sysproc.c`**: Wrote the main logic for the two new system calls (`sys_send` and `sys_recv`).
*   **`kernel/syscall.h` & `kernel/syscall.c`**: Registered the system call numbers and linked them to the system call table.
*   **`user/user.h` & `user/usys.pl`**: Added user-space definitions so programs can call `send()` and `recv()`.
*   **`user/msgtest.c`**: Created a new test program to demonstrate a parent sending messages to a child.
*   **`Makefile`**: Added `$U/_msgtest` so the test program compiles into the OS.

## New System Calls
1. **`int send(int pid, char *msg)`**
   * Drops a text message into the target `pid`'s mailbox. 
   * Returns `0` on success. If the mailbox is full (10 messages) or the PID is invalid, it returns `-1`.

2. **`int recv(char *buf)`**
   * Reads the oldest unread message from the caller's own mailbox into `buf`.
   * If the mailbox is empty, the process will go to sleep and wait until a message arrives.

## How to Test
Open your WSL terminal in the `xv6-riscv` folder and run:
1. `make clean`
2. `make qemu`
3. `$ msgtest` (inside the xv6 terminal)

The output will show a parent process successfully sending two messages to a child process:

**Expected Output:**
```text
$ msgtest
[parent] My PID is 3. Forked child with PID 4.
[child]  Received msg 1: Hello from the parent! (message 1)
[parent] Sent message 1 to child.
[child]  Received msg 2: This is the second message in the queue.
[parent] Sent message 2 to child.
[parent] Child finished. IPC test complete.
```
