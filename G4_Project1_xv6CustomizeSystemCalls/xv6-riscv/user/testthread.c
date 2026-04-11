#include "kernel/types.h"
#include "user/user.h"

void thread_func(){
	printf("Hello from thread!\n");
	exit(0);
}

int main(){
	void *stack = malloc(4096);
	int pid = clone(stack);

	if(pid == 0) thread_func(); 
	else{
		join(&stack);
		printf("Thread finished\n");
	}

	exit(0);
}
