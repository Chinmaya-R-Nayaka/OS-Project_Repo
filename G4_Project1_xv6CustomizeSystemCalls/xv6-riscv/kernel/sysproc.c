
#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"

extern struct proc proc[NPROC];
char *shared_mem = 0;
uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if(t == SBRK_EAGER || n < 0) {
    if(growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if(addr + n < addr)
      return -1;
    if(addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64 sys_hello(void){
	printf("Hello from kernel");
	return 0;
}


uint64 sys_getprocinfo(void){
	struct proc *p;
	
	printf("PID\tSTATE\n");	
	for(p = proc; p < &proc[NPROC]; p++){
		if(p->state != UNUSED) printf("%d\t%d\n", p->pid, p->state);
	}
	return 0;
}
uint64 sys_getyear(void){
  return 2026; 
}

/* --- Bhanu's Work: Message Passing IPC --- */

// sys_send(int receiver_pid, char *msg)
//
// Sends a text message to the mailbox of the process whose PID is receiver_pid.
// The message is copied from user space into the next free slot of the target's
// circular queue.
//
// Returns:
//   0  - message was successfully enqueued
//  -1  - receiver not found, receiver queue is full (10 msgs), or bad pointer
uint64
sys_send(void)
{
  int  receiver_pid;   // PID of the process we want to message
  uint64 uaddr;        // User-space pointer to the message string
  char kbuf[MSG_SIZE]; // Kernel-side buffer to hold the copied message

  // Argument 0: the integer PID of the intended receiver
  argint(0, &receiver_pid);
  // Argument 1: user-space pointer to the null-terminated message string
  argaddr(1, &uaddr);

  // Copy the message string from user space into our kernel buffer.
  // copyinstr returns -1 if the pointer is invalid or the string is too long.
  if(copyinstr(myproc()->pagetable, kbuf, uaddr, MSG_SIZE) < 0)
    return -1; // Bad user pointer or string too long

  // Walk the process table to find the target process by PID.
  // We briefly acquire each p->lock only to safely read PID and state.
  struct proc *receiver = 0;
  for(struct proc *p = proc; p < &proc[NPROC]; p++){
    acquire(&p->lock);
    if(p->pid == receiver_pid && p->state != UNUSED){
      receiver = p;
      release(&p->lock); // Release proc lock immediately — we use msglock below
      break;
    }
    release(&p->lock);
  }

  // If no live process with that PID was found, return error.
  if(receiver == 0)
    return -1;

  // Lock the receiver's message queue using the dedicated msglock.
  // We do NOT use receiver->lock here because sleep() acquires p->lock internally.
  // Using p->lock here would cause a double-acquire panic in sys_recv.
  acquire(&receiver->msglock);

  // Check whether the receiver's queue is full (already holds MAX_MSGS messages).
  // If full, we immediately return -1 (non-blocking fail) rather than sleeping.
  if(receiver->msg_count >= MAX_MSGS){
    release(&receiver->msglock);
    return -1; // Mailbox full — caller should retry or handle the failure
  }

  // Copy the kernel buffer into the receiver's queue at the tail position.
  memmove(receiver->msg_queue[receiver->msg_tail].textbox, kbuf, MSG_SIZE);

  // Advance the tail index with wraparound to maintain the circular structure.
  receiver->msg_tail = (receiver->msg_tail + 1) % MAX_MSGS;

  // Increment the count of unread messages in this mailbox.
  receiver->msg_count++;

  // Wake up the receiver if it was sleeping in sys_recv waiting for messages.
  // The wakeup channel must match the one used in sleep() inside sys_recv.
  wakeup(&receiver->msg_count);

  release(&receiver->msglock);
  return 0; // Message successfully enqueued
}

// sys_recv(char *buf)
//
// Dequeues and returns the oldest message from the calling process's mailbox.
// If the mailbox is empty, this call blocks (sleeps) until a sender delivers a message.
//
// The message text is written into the user-space buffer pointed to by buf.
//
// Returns:
//   0  - a message was successfully read into buf
//  -1  - bad user-space buffer pointer
uint64
sys_recv(void)
{
  uint64 uaddr;        // User-space pointer to the buffer that will receive the message
  char kbuf[MSG_SIZE]; // Kernel-side staging buffer to hold the dequeued message

  // Argument 0: user-space pointer to the destination buffer
  argaddr(0, &uaddr);

  struct proc *p = myproc();

  // Lock the message queue using the dedicated msglock (NOT p->lock).
  // KEY REASON: sleep(chan, lk) internally calls acquire(&p->lock) then release(lk).
  // If we passed p->lock as lk, it would try to acquire p->lock twice -> panic: acquire.
  // By using p->msglock as the condition lock, p->lock remains free for sleep() to use.
  acquire(&p->msglock);

  // If there are no messages yet, sleep until sys_send wakes us up.
  // We loop after each wakeup to guard against spurious wakeups.
  while(p->msg_count == 0){
    // sleep() atomically: acquires p->lock, releases p->msglock, sleeps.
    // On wakeup: releases p->lock, re-acquires p->msglock.
    sleep(&p->msg_count, &p->msglock);
  }

  // Dequeue the oldest message (at msg_head) into our kernel staging buffer.
  memmove(kbuf, p->msg_queue[p->msg_head].textbox, MSG_SIZE);

  // Advance the head index with wraparound to consume this slot.
  p->msg_head = (p->msg_head + 1) % MAX_MSGS;

  // Decrement the count of unread messages.
  p->msg_count--;

  release(&p->msglock);

  // Copy the dequeued message from the kernel buffer out to the user's buffer.
  if(copyout(p->pagetable, uaddr, kbuf, MSG_SIZE) < 0)
    return -1;

  return 0;
}

/* ----------------------------------------- */
/*Chandrima's work*/
/*---------------------------------------------*/
uint64 sys_shmget(void)
{
    if(shared_mem == 0){
        shared_mem = kalloc();
        if(shared_mem == 0)
            return -1;
    }
    return 1;
}

uint64 sys_shmat(void)
{
    int id;
    argint(0, &id);

    if(id != 1 || shared_mem == 0)
        return 0;

    struct proc *p = myproc();
    uint64 addr = PGROUNDUP(p->sz);

    if(mappages(p->pagetable, addr, PGSIZE,
        (uint64)shared_mem, PTE_W | PTE_R | PTE_U|PTE_SHM) < 0)
        return 0;

    p->sz += PGSIZE;
    return addr;
}
/*----------------------------------------------------*/

