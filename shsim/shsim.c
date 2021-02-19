/*Lewis Neibaur
*cs3337
*this program simulates a sh terminal
*created from instuctions in chapter 3.11 of 'Systems Programming in Unix/Linux' by K.C. Wang
*/
#include "shsim.h"
char strOut[128]; 	//global variable for use in logdebug
int tabs;			//global variable for use in logdebug
char logPath[128];	//global variable for use in logdebug

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
		getInput(line, &argCount, &argVector);	//get input from user
		if( argVector[0] && !strcmp(argVector[0],"exit")){		//if first argument is "exit"
			logDebug("exiting program", tabs);
			exit(0);							//exit program
		}
		else if (argVector[0] && !strcmp(argVector[0],"cd")){	//if first argument is "cd"
			logDebug("changing directory", tabs);
			int error;
			if (argVector[1])					//if there is a seccond argument
				error = chdir(argVector[1]);	//try to change to path in 2nd arg
			else								//if 2nd arg was null
				error = chdir("~");				//try to change cwd to home
			if (error)							// if cd was not successful
				printf("error: invalid directory path: %s\n", argVector[1]);	// display error
		}
		else{									//for all other commands
			logDebug("other command", tabs);



		}
		free(argVector);						//dealocate memory from argVector array before repeating loop
	}	
}

/****************************************function definitions*******************************************/



/***********************functions for getting user input***************************/
//get a line of input from user and store in argCount and argVector
//input should be formatted as cmd arg1 arg2 arg3 .... argn
void getInput( char *line, int *argCount, char ***argVector){     //get a line of input from user
	tabs ++;
	logDebug("getInput()", tabs);
	getInputLine(line);							//get user input
	setArgcSim(line, argCount);					//count number of arguments
	setArgvSim(line, argCount, argVector);		//store arguments in argVector
	tabs --;
}
//get line of input from user
void getInputLine(char *line){
	tabs ++;
	char msg[128];
	getcwd(msg, 128);
	printf("%s:%s $ ", getlogin(), msg );       //display message to user
    fgets(line, 128, stdin);                    //get input line from user
	line[strlen(line)-1] = 0;                   //kill \n at the end of line
	sprintf(strOut, "got input: %s", line); 
	logDebug(strOut, tabs);
	tabs --;
}
//count the number of arguments from line
void setArgcSim(char *line, int *argCount){
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
void setArgvSim(char *line, int *argCount, char ***argVector){
	tabs++;
	char ** newStrArray = (char **)malloc((*argCount + 1) * sizeof(char *)); //create new string array
	char *token;								//string to hold tokens
	token = strtok(line, " ");					//get first token
	int i = 0; 
	while(i < *argCount){						//loop through tokens and array
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
void logReset(){
	tabs = 0;
	getcwd(logPath, 128);
	strcat(logPath, "/debug.log");
	FILE *fp = fopen(logPath, "w");		// fopen a FILE stream for 
    fprintf(fp, "****new log****\n"); 	// add message
    fclose(fp);
}
//add str to debug.log file for easy debugging
void logDebug(char *str, int tabs){
	FILE *fp = fopen(logPath, "a");		// fopen a FILE stream for APPEND
	for (int i = 0; i < tabs; i++){
		fprintf(fp, "\t");
	}
	fprintf(fp, "%s\n",str);				// append str to file
	fclose(fp);								// close FILE stream when done
}
//log argc, argv, and env
void logArgEnv(int argc, char *argv[], char *env[ ]){
	logDebug("logArgEnv()--------------", tabs);
	if (argc){
		sprintf(strOut, "argc = %d", argc);
		logDebug(strOut, tabs);
	}
	if (argv){
		sprintf(strOut, "argvPtr = %p", argv);
		logDebug(strOut, tabs);
	}
	int i = 0;
	while(argv && argv[i]){ 
		sprintf(strOut, "argv[%d] = %s", i, argv[i]); 
		logDebug(strOut, tabs);
		i++; 
	}
	if (argv && argc){
		sprintf(strOut, "argv[%d] = %s", argc, argv[argc]);
		logDebug(strOut, tabs);
	}
	logDebug("m", tabs);
	i = 0; 
	while(env && env[i]){
		logDebug("n", tabs); 
		sprintf(strOut, "env[%d] = %s", i, env[i]);
		logDebug("m", tabs);
		//logDebug (strOut, tabs);
		i++;
	} 
}