#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>	
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>

void strTrim(char* str) {
	int readIndex, writeIndex = 0;
	while (str[readIndex] == ' ') {			//loop past all leading spaces
		printf("skip[%i]", readIndex);
		readIndex++;
	}
	while (str[readIndex] != '\0') {			//loop till end of string
		if (str[readIndex] == '<' || str[readIndex] == '>') { //if < or >
			while (str[readIndex] == ' ') {
				readIndex++;				//skip spacesafter > or <
			}
		}
		if (str[readIndex] != ' ') {		//if character is not a space
			str[writeIndex] = str[readIndex];	//copy read to write
			writeIndex++;
		}
		else {								//if character is a space
			if (str[writeIndex - 1] != ' ') {	//only copy first space
				str[writeIndex] = str[readIndex];	//copy read to write
				writeIndex++;
			}
		}
		readIndex++;
		printf("		|%s|\n", str);
	}
	str[writeIndex] = '\0';					//end string with null character
}


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

/*	FILE* fp = fopen("outfile", "w");		// fopen a FILE stream for 
	fclose(fp);
	close(1); open("outfile", O_WRONLY, 0644);
	char* cmd = "/bin/echo";
	char* argVector[] = { "echo", "hello", "world", NULL };
	int r = execve(cmd, argVector, env);
	printf("didnt work\n");
*/

/*	close(1); open("outfile", O_WRONLY|O_CREAT, 0644);
	char* cmd = "/bin/echo";
	char* argVector[] = { "echo", "hello", "world", NULL };
	int r = execve(cmd, argVector, env);
	printf("didnt work\n");
*/

/*	close(1); open("outfile", O_WRONLY|O_CREAT|O_APPEND, 0644);
	char* cmd = "/bin/echo";
	char* argVector[] = { "echo", "hello", "world", NULL };
	int r = execve(cmd, argVector, env);
	printf("didnt work\n");
*/

/*	close(0); open("infile", O_RDONLY);
	char* cmd = "/bin/cat";
	char* argVector[] = { "cat", NULL };
	int r = execve(cmd, argVector, env);
	printf("didnt work\n");
*/

	char* input = "    test this  function > to see>if  >   it >> works < very well    ";
	printf("|%s|\n", input);
	strTrim(input);
	printf("strTrim()\n|%s|\n", input);



}

