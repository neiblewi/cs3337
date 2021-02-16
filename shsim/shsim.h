#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>	

//functions declarations
/********functions for use in main function*******/
int initialize();		//initialize program

/*****************getinput***************************/
int getInput(int *argcSim, char **argvSim);			//get a line of input from user
int getInputLine(char * line);
int setArgcSim(char *line, int *argcSim);
int setArgvSim(char *line, int *argcSim, char **argvSim);

/********functions for writing to debug.log file**********/
int logReset();			//resets debug.log file
int logDebug(char *str);		//write a line to log file for debuging
int logArgEnv(int argc, char *argv[], char *env[ ]);	//prints the arguments passed into main