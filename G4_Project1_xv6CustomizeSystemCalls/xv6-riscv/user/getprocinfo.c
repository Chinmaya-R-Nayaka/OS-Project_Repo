#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void){
    getprocinfo();   // syscall
    exit(0);
}
