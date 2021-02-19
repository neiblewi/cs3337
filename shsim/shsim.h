#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>	
#include <stdarg.h>
#include <unistd.h>

//functions declarations
/********functions*******/

/*********funtions for getting input from user**************/
void getInput(char * line, int *argCount, char ***argVector);	//get a line of input from user and store arguments
void getInputLine(char * line);								//get a line from user	
void setArgcSim(char *line, int *argCount);					//count number of arguments
void setArgvSim(char *line, int *argCount, char ***argVector);	//store arguments

/********functions for writing to debug.log file**********/
void logReset();											//resets debug.log file
void logDebug(char *str, int tabs);						//write a line to log file for debuging
void logArgEnv(int argc, char *argv[], char *env[ ]);	//prints the arguments passed into main