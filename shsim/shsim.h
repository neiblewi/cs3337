#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>	
#include <stdarg.h>

//functions declarations
/********functions*******/

/*********funtions for getting input from user**************/
int getInput(char * line, int *argCount, char ***argVector);	//get a line of input from user and store arguments
int getInputLine(char * line);								//get a line from user	
int setArgcSim(char *line, int *argCount);					//count number of arguments
int setArgvSim(char *line, int *argCount, char ***argVector);	//store arguments

/********functions for writing to debug.log file**********/
int logReset();											//resets debug.log file
int logDebug(char *str, int tabs);						//write a line to log file for debuging
int logArgEnv(int argc, char *argv[], char *env[ ]);	//prints the arguments passed into main