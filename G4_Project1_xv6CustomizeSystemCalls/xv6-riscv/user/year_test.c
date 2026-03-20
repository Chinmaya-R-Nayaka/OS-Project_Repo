#include "kernel/types.h"
#include "user/user.h"

int main() {
    printf("The current year in xv6 is: %d\n", getyear());
    exit(0);
}
