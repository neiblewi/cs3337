#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>	
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>

//functions declarations
/********functions*******/
void cd(char *path);																		//changes cwd to path
void forkChild(int argCount, char** argVector, char** env, char* redirPath, int inOut);		//forks a child process
void executeCommand(int argCount, char **argVector, char **env);							// change process image to command specified

/*********funtions for getting input from user**************/
void getInput(char* line, int* argCount, char*** argVector, char** redirPath, int* inOut);	//get a line of input from user and store arguments
void handleRedirect(char* line, char** redirPath, int *inOut);								//
void searchStr(char* head, char delimiter, char** tail);									//
void getInputLine(char * line);																//get a line from user	
void strArrCount(char* line, int* argCount, char delimiter);								//count number of arguments
void strSplit(char *line, int *argCount, char ***argVector, char delimiter);				//store arguments

/********functions for writing to debug.log file**********/
void logReset();													//resets debug.log file
void logDebug(char *str, int tabs);									//write a line to log file for debuging
void logStrArray(int arrLength, char* strArray[], char* title);		//logs the array passed to function