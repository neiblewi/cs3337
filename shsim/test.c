#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>	
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>

void strTrim(char* str) {
	int readIndex = 0;
	int writeIndex = 0;
	while (str[readIndex] == ' ') {			//loop past all leading spaces
		printf("skip[%i]\n", readIndex);
		readIndex++;
	}
	while (str[readIndex] != '\0') {			//loop till end of string
		printf("	|%s|\n", str);
		if (str[readIndex] == '<' || str[readIndex] == '>') { //if < or >
			printf("<>\n");
			while (str[readIndex] == ' ') {
				readIndex++;				//skip spacesafter > or <
			}
		}
		if (str[readIndex] == ' ' && str[readIndex + 1] == ' ') {	//if there are two spaces in a row
			printf("|  |\n");
			readIndex++;					//skip extra whitespace
		}
		else{									//if other character or single space
			printf("other\n");
			printf("write[%i]%c\n", writeIndex, str[writeIndex]);
			printf("write[%i]%c = read[%i]%c\n", writeIndex, str[writeIndex], readIndex, str[readIndex]);
			*str[writeIndex] = *str[readIndex];	//copy read to write
			writeIndex++;
		}
		readIndex++;
		printf("end\n;")
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

