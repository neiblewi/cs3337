#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>	
#include <stdarg.h>
#include <unistd.h>

//functions declarations
/********functions*******/
void cd(char *path);												//changes cwd to path
void forkChild(int argCount, char **argVector, char **env);			//forks a child process
void executeCommand(int argCount, char **argVector, char **env);	// change process image to command specified
//void getPaths(char *cmd, char* arg0, char **env);						//gets a path to command in arg0 from path in env

/*********funtions for getting input from user**************/
void getInput(char * line, int *argCount, char ***argVector);	//get a line of input from user and store arguments
void getInputLine(char * line);									//get a line from user	
void setArgCount(char *line, int *argCount);						//count number of arguments
void strSplit(char *line, int *argCount, char ***argVector, char delimiter);	//store arguments

/********functions for writing to debug.log file**********/
void logReset();											//resets debug.log file
void logDebug(char *str, int tabs);							//write a line to log file for debuging
void logArgEnv(int argc, char *argv[], char *env[ ]);		//prints the arguments passed into main
//void logArray(char** arr, int arrLength);