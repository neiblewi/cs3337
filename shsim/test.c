#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>	
#include <stdarg.h>
#include <unistd.h>
int main(int argc, char* argv[], char* env[]) {
	char* cmd = "echo";
	char* argVector [] = { "echo", "hello", "world", NULL };
	int r = execve(cmd, argVector, env);
}

