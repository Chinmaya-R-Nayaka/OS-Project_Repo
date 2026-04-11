
#ifndef SPINLOCK_H
#define SPINLOCK_H

struct spinlock {
  uint locked;
  char *name;
  struct cpu *cpu;
};

#endif
