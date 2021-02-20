/********************* C3.1. c: fork() ************************/ 

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>  

char *dir[64], *myargv[ 64]; // assume at most 64 parameters 
char cmd[128]; 

int main( int argc, char *argv[], char *env[]) { 
	int pid, status; 
	printf(" THIS IS %d MY PARENT =%d\n", getpid(), getppid()); 
	pid = fork(); // fork syscall; parent returns child pid, 
	if (pid){ // PARENT EXECUTES THIS PART 
		printf(" THIS IS PROCESS %d CHILD PID = %d\n", getpid(), pid);
		printf(" PARENT %d WAITS FOR CHILD %d TO DIE\n", getpid(), pid); 
		pid = wait(&status); // wait for ZOMBIE child process 
		printf(" DEAD CHILD =% d, status = 0x% 04x\n", pid, status); 
	} 
	else{ // child executes this part (3). 
		printf(" this is process %d parent =%d\n", getpid(), getppid()); 
		int i, r; 
		printf("THIS IS PROCESS %d IN %s\n", getpid(), argv[ 0]); 
		if (argc < 2){ 
			printf("Usage: a.out command [options]\n"); 
			exit(0); 
		} 
		printf("argc = %d\n", argc); 
		for (i = 0; i < argc; i++) // print argv[ ] strings 
			printf("argv[% d] = %s\n", i, argv[ i]); 
		for (i = 0; i < argc - 1; i++) // create myargv[ ] 
			myargv[ i] = argv[ i + 1]; 
		myargv[i] = 0; // NULL terminated array 
		strcpy(cmd, "/bin/"); // create /bin/ command 
		strcat(cmd, myargv[ 0]); 
		printf("cmd = %s\n", cmd); // show filename to be executed 
		r = execve( cmd, myargv, env); // come to here only if execve() failed 
		printf("execve() failed: r = %d\n", r); 
		printf(" child %d dies by exit( VALUE)\n", getpid()); 
		exit( 100); 
	} 
}