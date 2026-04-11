#ifndef MUTEX_H
#define MUTEX_H
#include "spinlock.h"

struct mutex {
    int locked;
    struct spinlock lk;
};

void mutex_init(struct mutex *m);
void mutex_acquire(struct mutex *m);
void mutex_release(struct mutex *m);

#endif
