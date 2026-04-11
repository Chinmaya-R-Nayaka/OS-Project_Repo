#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "spinlock.h"
#include "mutex.h"

void mutex_init(struct mutex *m){
    initlock(&m->lk, "mutex");
    m->locked = 0;
}

void mutex_acquire(struct mutex *m){
    acquire(&m->lk);

    while(m->locked){
        sleep(m, &m->lk);
    }

    m->locked = 1;
    release(&m->lk);
}

void mutex_release(struct mutex *m){
    acquire(&m->lk);

    m->locked = 0;
    wakeup(m);

    release(&m->lk);
}
