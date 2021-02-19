/*Lewis Neibaur
*cs3337
*this program simulates a sh terminal
*created from instuctions in chapter 3.11 of 'Systems Programming in Unix/Linux' by K.C. Wang
*/
#include "shsim.h"
char strOut[128]; 	//global variable for use in logdebug
int tabs;			//global variable for use in logdebug

//main function
int main( int argc, char *argv[], char *env[ ]){
	logReset();							
	logDebug("\nmain function:", tabs);	
	logArgEnv(argc, argv, env);			
	int i = 1;							
	while(i){									//main program loop
		int argCount;							//empty integer to simulate argc
		char **argVector = NULL;				//empty pointer to array of strings to simulate argv
		char line[128];							//string to hold user input line
		getInput(&argCount, &argVector, line);	//get input from user
		logArgEnv(argCount, argVector, NULL); 	
		free(argVector);						//dealocate memory from argVector array before repeating loop
	}	
}

/****************************************function definitions*******************************************/




//get a line of input from user and store in argCount and argVector
//input should be formatted as cmd arg1 arg2 arg3 .... argn
int getInput(int *argCount, char ***argVector, char * line){     //get a line of input from user
	tabs ++;
	logDebug("getInput()", tabs);
	getInputLine(line);							//get user input
	setArgcSim(line, argCount);					//count number of arguments
	setArgvSim(line, argCount, argVector);		//store arguments in argVector
	tabs --;
}
//get line of input from user
int getInputLine(char *line){
	tabs ++;
	printf("enter command:");                   //display message to user
    fgets(line, 128, stdin);                    //get input line from user
	line[strlen(line)-1] = 0;                   //kill \n at the end of line
	sprintf(strOut, "got input: %s", line); 
	logDebug(strOut, tabs);
	tabs --;
}
//count the number of arguments from line
int setArgcSim(char *line, int *argCount){
	tabs++;
	*argCount = 1;								//count how many args are in input. need n+1
	for(int i = 0; i < strlen(line); i++){		//loop throught input char by char
		if(line[i] == (int)' '){ 				//find ' '
		*argCount = *argCount + 1;				//count how many ' ' are in user input line
		}
	}	
	sprintf(strOut, "argCount= %i", *argCount); 
	logDebug(strOut, tabs);
	tabs --;
}
//tokenize and store arguments from line
int setArgvSim(char *line, int *argCount, char ***argVector){
	tabs++;
	char ** newStrArray = (char **)malloc((*argCount + 1) * sizeof(char *)); //create new string array
	char *token;								//string to hold tokens
	token = strtok(line, " ");					//get first token
	int i = 0; 
	while(token && i < *argCount){				//loop through tokens and array
		//strcpy(newStrArray[i], token);
		newStrArray[i] = token;					//store token
		i++;									//go to next index in array
		token = strtok(0, " ");					//go to next token
	}
	newStrArray[*argCount] = NULL;				//last arg points to null
	*argVector = newStrArray;					//change argVector to point to new array
	logArgEnv(*argCount, *argVector, NULL);
	tabs --;
}

/******************functions for writing to debug.log file***********************/

//reset debug.log file
int logReset(){
	FILE *fp = fopen("debug.log", "w");		// fopen a FILE stream for 
    fprintf(fp, "\n****new log****\n"); 	// add message
    fclose(fp);								// close file
	tabs = 0;
}

//add str to debug.log file for easy debugging
int logDebug(char *str, int tabs){
	FILE *fp = fopen("debug.log", "a");		// fopen a FILE stream for APPEND
    for (int i = 0; i < tabs; i++){
		fprintf(fp, "\t");
	}
	fprintf(fp, "%s\n",str);				// append str to file
    fclose(fp);								// close FILE stream when done
}

//log argc, argv, and env
int logArgEnv(int argc, char *argv[], char *env[ ]){
	logDebug("logArgEnv()--------------");
	sprintf(strOut, "argc = %d", argc);
	logDebug(strOut, tabs);

	sprintf(strOut, "argvPtr = %p", argv);
	logDebug(strOut, tabs);
	int i = 0;
	while(argv[i]){ 
		sprintf(strOut, "argv[%d] = %s", i, argv[i]); 
		logDebug(strOut, tabs);
		i++; 
	}
	sprintf(strOut, "argv[%d] = %s", argc, argv[argc]); 
	logDebug(strOut, tabs);
	/*i = 0; 
	while(env[i]){ 
		sprintf(strOut, "env[%d] = %s", i, env[i]); 
		logDebug (strOut, tabs);
		i++; 
	}	*/ //seg fault needs fixed when env becomes important
}