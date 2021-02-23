#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>	
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[], char* env[]) {
/*	char* cmd = "/bin/echo";
	char* argVector [] = { "echo", "hello", "world", NULL };
	int r = execve(cmd, argVector, env);
	printf("didnt work\n");
*/
/*	char* cmd = "/usr/bin/gcc";
	char* argVector[] = { "gcc", "t.c", NULL };
	int r = execve(cmd, argVector, env);
	printf("didnt work\n");
*/
	
//o_wronly requires file to exist
/*	close(1); open("outfile", O_WRONLY, 0644);
	char* cmd = "/bin/echo";
	char* argVector[] = { "echo", "hello", "world", NULL };
	int r = execve(cmd, argVector, env);
	printf("didnt work\n");
*/

	FILE* fp = fopen("outfile", "w");		// fopen a FILE stream for 
	fclose(fp);
	close(1); open("outfile", O_CREAT, 0644);
	char* cmd = "/bin/echo";
	char* argVector[] = { "echo", "hello", "world", NULL };
	int r = execve(cmd, argVector, env);
	printf("didnt work\n");
}

