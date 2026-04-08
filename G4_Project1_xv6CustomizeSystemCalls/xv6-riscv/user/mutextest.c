#include "kernel/types.h"
#include "user/user.h"

int main(){
    int pid;

    for(int i=0;i<5;i++){
        pid = fork();

        if(pid==0){
            for(int j=0;j<1000;j++){
                mutex_test_inc();
            }
            exit(0);
        }
    }

    for(int i=0;i<5;i++)
        wait(0);

    printf("Final counter = %d\n", get_counter());

    exit(0);
}