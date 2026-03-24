#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>

#define SIZE 200

int main(){
	char buffer[SIZE]; // To Store the Commands Entered by the user
	char *args[25]; // Arguments array --> Stores individual Commands

	while(1){
		printf("G4_Shell> "); // Custom Shell
		fgets(buffer, sizeof(buffer), stdin); // Taking  Input from user

		buffer[strcspn(buffer, "\n")] = 0; // Removing New Line
		if(strcmp(buffer, "exit") == 0) break; // Exit Condition

		// Spliting the commands 
		int i = 0;
		char *command = strtok(buffer, " ");
		while(command != NULL){
			args[i] = command;
			command = strtok(NULL, " ");
			i++;
		}
		args[i] = NULL; // Marks the End of arguments list
		

		pid_t a = fork();
		if(a == 0){
			// Child Process
			execvp(args[0], args);

			// if exec fails
			printf("Command Not Found!!\n");
		}
		else wait(NULL); // Parent Process
	}
	return 0;
}
