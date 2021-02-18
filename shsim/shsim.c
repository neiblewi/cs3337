/*Lewis Neibaur
*cs3337
*this program simulates a sh terminal
*created from instuctions in chapter 3.11 of 'Systems Programming in Unix/Linux' by K.C. Wang
*/
#include "shsim.h"
char strOut[128]; //global variable for use in logdebug

//main function
int main( int argc, char *argv[], char *env[ ]){
	logReset();							
	logDebug("\nmain function:");	
	logArgEnv(argc, argv, env);			
	
	int argcSim;						//empty integer to simulate argc
	char **argvSim;						//empty pointer to array of strings to simulate argv
	int i = 1;							
	while(i){							//main program loop
		sprintf(strOut, "---\t**argvsim= %p", argcSim); logDebug(strOut);
		getInput(&argcSim, &argvSim);	//get input from user
		sprintf(strOut, "---\t**argvsim= %p", argcSim); logDebug(strOut);
		logArgEnv(argcSim, argvSim, NULL); 	//log user input
	}	
}

/****************************************function definitions*******************************************/




//get a line of input from user and store in argcSim and argvSim
//input should be formatted as cmd arg1 arg2 arg3 .... argn
int getInput(int *argcSim, char ***argvSim){     //get a line of input from user
	logDebug("\tgetInput()");
	char line[128];				                //string to hold user input line
	sprintf(strOut, "---\t***argvsim= %p,	**argvsim = %p", argcSim, *argcSim); logDebug(strOut);
	getInputLine(line);
	setArgcSim(line, argcSim);
	setArgvSim(line, argcSim, argvSim);
	sprintf(strOut, "---\t***argvsim= %p,	**argvsim = %p", argcSim, *argcSim); logDebug(strOut);
	logDebug("\treturn from getInput()\n");
}

int getInputLine(char *line){
	printf("enter command:");                   //display message to user
    fgets(line, 128, stdin);                    //get input line from user
	line[strlen(line)-1] = 0;                   //kill \n at the end of line
    sprintf(strOut, "\tgot input: %s", line); logDebug(strOut);
}

int setArgcSim(char *line, int *argcSim){
	*argcSim = 1;								//count how many args are in input. need n+1
	for(int i = 0; i < strlen(line); i++){		//loop throught input char by char
		if(line[i] == (int)' '){ 				//find ' '
		*argcSim = *argcSim + 1;				//count how many ' ' are in user input line
		}
	}	
	sprintf(strOut, "\targcSim= %i", *argcSim); logDebug(strOut);
}

int setArgvSim(char *line, int *argcSim, char ***argvSim){
	char ** newStrArray = (char **)malloc((*argcSim + 1) * sizeof(char *)); //create new string array
	sprintf(strOut, "---\t***argvsim= %p,	**argvsim = %p,	**newArr= %p", argcSim, *argcSim, newStrArray); logDebug(strOut);
	
	char *token;								//string to hold tokens
	token = strtok(line, " ");					//get first token
	int i = 0; 
	while(token && i < *argcSim){				//loop through tokens and array
		newStrArray[i] = token;					//store token
		i++;									//go to next index in array
		token = strtok(0, " ");					//go to next token
	}
	newStrArray[*argcSim] = NULL;				//last arg points to null

	sprintf(strOut, "---\t***argvsim= %p,	**argvsim = %p,	**newArr= %p", argcSim, *argcSim, newStrArray); logDebug(strOut);
	int temp = (int)newStrArray;
	*argvSim = newStrArray;						//change argvsim to point to new array
	sprintf(strOut, "---\t***argvsim= %p,	**argvsim = %p,	**newArr= %p, i:temp= %i, p:temp= %p", argcSim, *argcSim, newStrArray, temp, temp); logDebug(strOut);
	
	logArgEnv(*argcSim, *argvSim, NULL);
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
	sprintf(strOut, "argc = %d", argc);logDebug(strOut);
	int i = 0;
	while(argv[i]){ 
		sprintf(strOut, "argv[%d] = %s", i, argv[i]); logDebug(strOut);
		i++; 
	}
	sprintf(strOut, "argv[%d] = %s", argc, argv[argc]); logDebug(strOut);
	/*i = 0; 
	while(env[i]){ 
		sprintf(strOut, "env[%d] = %s", i, env[i]); 
		logDebug (strOut);
		i++; 
	}	*/ //seg fault needs fixed when env becomes important
}