/*********** C3.6. c file: compile and run a.out ***********/ 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

char *dir[64], *myargv[ 64]; // assume at most 64 parameters 
char cmd[128]; 

int main( int argc, char *argv[], char *env[]) { 
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
}