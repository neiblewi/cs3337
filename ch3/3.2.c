/***************** C3.2. c file ********************/ 
#include < stdio.h > 

int main() { 
	int pid = fork(); // fork a child 
	if (pid){ // PARENT 
		printf(“ PARENT %d CHILD =% d\ n”, getpid(), pid); 
		// sleep( 1); // sleep 1 second = = > let child run next 
		printf(" PARENT %d EXIT\ n", getpid()); 
	} 
	else{ // child 
		printf(" child %d start my parent =% d\ n", getpid(), getppid()); 
		// sleep( 2); // sleep 2 seconds = > let parent die first 
		printf(" child %d exit my parent =% d\ n", getpid(), getppid()); 
	} 
}