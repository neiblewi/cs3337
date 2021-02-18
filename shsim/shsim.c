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
	
	int argCount;						//empty integer to simulate argc
	char **argVector;					//empty pointer to array of strings to simulate argv
	char ***argVectorPtr = &argVector;
	int i = 1;							
	while(i){							//main program loop
		sprintf(strOut, "**argVector= %p", argVector); logDebug(strOut, tabs);
		getInput(&argCount, argVectorPtr);	//get input from user
		sprintf(strOut, "**argVector= %p", argVector); logDebug(strOut, tabs);
		logArgEnv(argCount, argVector, NULL); 	//log user input
	}	
}

/****************************************function definitions*******************************************/




//get a line of input from user and store in argCount and argVector
//input should be formatted as cmd arg1 arg2 arg3 .... argn
int getInput(int *argCount, char ***argVector){     //get a line of input from user
	tabs ++;
	logDebug("getInput()", tabs);
	sprintf(strOut, "*(**argVector)= %p,	**argVector = %p", argVector, *argVector); 
	logDebug(strOut, tabs);
	
	char line[128];				                //string to hold user input line
	getInputLine(line);							//get user input
	setArgcSim(line, argCount);					//count number of arguments
	setArgvSim(line, argCount, argVector);			//store arguments in argVector
	
	sprintf(strOut, "*(**argVector)= %p,	**argVector = %p", argVector, *argVector); 
	logDebug(strOut, tabs);
	logArgEnv(*argCount, *argVector, NULL);
	logDebug("return from getInput()\n", tabs);
	tabs --;
}

int getInputLine(char *line){
	tabs ++;
	printf("enter command:");                   //display message to user
    fgets(line, 128, stdin);                    //get input line from user
	line[strlen(line)-1] = 0;                   //kill \n at the end of line
    
	sprintf(strOut, "got input: %s", line); 
	logDebug(strOut, tabs);
	tabs --;
}

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

int setArgvSim(char *line, int *argCount, char ***argVector){
	tabs++;
	
	char ** newStrArray = (char **)malloc((*argCount + 1) * sizeof(char *)); //create new string array
	
	sprintf(strOut, "*(**argVector)= %p,	**argVector = %p,	**newArr= %p", argVector, *argVector, newStrArray); 
	logDebug(strOut, tabs);
	
	char *token;								//string to hold tokens
	token = strtok(line, " ");					//get first token
	int i = 0; 
	while(token && i < *argCount){				//loop through tokens and array
		newStrArray[i] = token;					//store token
		i++;									//go to next index in array
		token = strtok(0, " ");					//go to next token
	}
	newStrArray[*argCount] = NULL;				//last arg points to null
	
	sprintf(strOut, "*(**argVector= %p),	**argVector = %p,	**newArr= %p", argVector, *argVector, newStrArray); 
	logDebug(strOut, tabs);
	
	*argVector = newStrArray;						//change argVector to point to new array
	
	sprintf(strOut, "*(**argVector= %p),	**argVector = %p,	**newArr= %p", argVector, *argVector, newStrArray); 
	logDebug(strOut, tabs);
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
	sprintf(strOut, "argc = %d", argc);
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