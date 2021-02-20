/********************* C3.1. c: fork() ************************/ 
#include <stdio.h> 

int main() { 
	int pid; 
	printf(" THIS IS %d MY PARENT =%d\n", getpid(), getppid()); 
	pid = fork(); // fork syscall; parent returns child pid, 
	if (pid){ // PARENT EXECUTES THIS PART 
		printf(" THIS IS PROCESS %d CHILD PID = %d\n", getpid(), pid);
	} 
	else{ // child executes this part (3). 
		printf(" this is process %d parent =%d\n", getpid(), getppid()); 
	} 
}