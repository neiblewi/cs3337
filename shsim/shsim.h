#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>	

//functions declarations
/********functions for use in main function*******/
int initialize();		//initialize program
int getInput();			//get a line of input from user

/********functions for writing to debug.log file**********/
int logReset();			//resets debug.log file
int log(char *str);		//write a line to log file for debuging
int logArgEnv(int argc, char *argv[], char *env[ ]);	//prints the arguments passed into main