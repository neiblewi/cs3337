/*Lewis Neibaur
*cs3337
*this program simulates a sh terminal
*created from instuctions in chapter 3.11 of 'Systems Programming in Unix/Linux' by K.C. Wang
*/
#include "shsim.h"

//main function
int main( int argc, char *argv[], char *env[ ]){
	logReset();							//clear log file
	logDebug("\nmain function:");	
	logArgEnv(argc, argv, env);			//log arguments and environment from main functions
	int argcSim;						//empty integer to simulate argc
	char *argvSim[];						//empty pointer to array of strings to simulate argv
	int i = 1;							
	while(i){							//main program loop
		logDebug("	in main while");
		getInput(&argcSim, &argvSim);	//get input from user
		logDebug("\nargcSim, argvSim from input:"); 
		logArgEnv(argcSim, argvSim, NULL); 	//log user input
	}	
}

/****************************************function definitions*******************************************/
//initialize program variables
int initialize(){
	
}

//get a line of input from user and store in argcSim and argvSim
//input should be formatted as cmd arg1 arg2 arg3 .... argn
int getInput(int *argcSim, char **argvSim){     //get a line of input from user
	logDebug("\ngetInput()");
	char line[128], strOut[128];                //string to hold user input line
	printf("enter command:");                   //display message to user
    fgets(line, 128, stdin);                    //get input line from user
    logDebug("got input:");
	line[strlen(line)-1] = 0;                   //kill \n at the end of line
    logDebug(line);
	*argcSim = 1;								//count how many args are in input. need n+1
	for(int i = 0; i < strlen(line); i++){		//loop throught input char by char
		if(line[i] == (int)' '){ 				//find ' '
		sprintf(strOut, "found ' ' at index %d", i); 
		logDebug(strOut);
		*argcSim = *argcSim + 1;				//count how many " " are in user input line
		}
	}
	argvSim = (char **)malloc((*argcSim + 1) * sizeof(char *));	//make argv point to new array
	logDebug("fill array");
	char *token;								//string to hold tokens
	token = strtok(line, " ");					//get first token
	int i = 0;
	while(token && i < *argcSim){				//loop through tokens and array
		argvSim[i] = token;						//store token
		i++;									//go to next index in array
		token = strtok(0, " ");					//go to next token
		logDebug("add arg");
	}
	argvSim[*argcSim] = NULL;	//last arg points to null
	logDebug("array full");
	logArgEnv(*argcSim, argvSim, NULL);
	logDebug("return from getInput()");
}

/******************functions for writing to debug.log file***********************/

//reset debug.log file
int logReset(){
	FILE *fp = fopen("debug.log", "w");		// fopen a FILE stream for 
    fprintf(fp, "\n****new log****\n"); 	// add message
    fclose(fp);								// close file
}

//add str to debug.log file for easy debugging
int logDebug(char *str){
	FILE *fp = fopen("debug.log", "a");		// fopen a FILE stream for APPEND
    fprintf(fp, "%s\n",str);				// append str to file
    fclose(fp);								// close FILE stream when done
}

//log argc, argv, and env
int logArgEnv(int argc, char *argv[], char *env[ ]){
	char strOut[128];
	sprintf(strOut, "argc = %d", argc);
	logDebug(strOut);
	int i = 0;
	while(argv[i]){ 
		sprintf(strOut, "argv[%d] = %s", i, argv[i]); 
		logDebug(strOut);
		i++; 
	}
	sprintf(strOut, "argv[%d] = %s", argc, argv[argc]); 
	logDebug(strOut);
	/*i = 0; 
	while(env[i]){ 
		sprintf(strOut, "env[%d] = %s", i, env[i]); 
		logDebug (strOut);
		i++; 
	}	*/ //seg fault needs fixed when env becomes important
}