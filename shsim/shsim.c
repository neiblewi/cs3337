/*Lewis Neibaur
*cs3337
*this program simulates a sh terminal
*created from instuctions in chapter 3.11 of 'Systems Programming in Unix/Linux' by K.C. Wang
*/
#include "shsim.h"

//main function
int main( int argc, char *argv[], char *env[ ]){
	logReset();							//clear log file
	log("\nmain function:")	
	logArgEnv(argc, argv, env);			//log arguments and environment from main functions
	int argcSim;						//empty integer to simulate argc
	char *argvSim[];					//empty pointer to array of strings to simulate argv
	int i = 1;							
	while(i){							//main program loop
		getInput(argcSim, argvSim);			//get input from user
		log("\nargcSim, argvSim from input:") 
		logArgEnv(argcSim, argvSim, NULL); 	//log user input
	}	
}

/****************************************function definitions*******************************************/
//initialize program variables
int initialize(){
	
}

//get a line of input from user and store in argcSim and argvSim
//input should be formatted as cmd arg1 arg2 arg3 .... argn
int getInput(int argcSim, char *argvSim[]){     //get a line of input from user
	char line[128];                             //string to hold user input line
	printf("enter command:");                   //display message to user
    fgets(line, 128, stdin);                    //get input line from user
    log("\ngot input:");
    log(line);
	line[strlen(line)-1] = 0;                   //kill \n at the end of line
	argcSim = 1;								//count how many args are in input. need n+1
	for(int i = 0; i < strlen(line); i++){		//loop throught input char by char
		if(strcmp(line[i]," ")) argcSim++;		//count how many " " are in user input line
	}
	char *newArray[argcSim];					//create a new array of string pointers of size argc
	argvSim = newArray;							//make argv point to new array

	char temp[128];								//temporary string to to hold line
	strcpy(temp, line);
	char *token;
	token = strtok(temp, " ");
	int i = 0;
	while(token && i < argcSim){
		argvSim[i] = token;
		i++;
		token = strtok(0, " ");
	}
}

/******************functions for writing to debug.log file***********************/

//reset debug.log file
int logReset(){
	FILE *fp = fopen("debug.log", "a");		// fopen a FILE stream for 
    fprintf(fp, "\n\n\n\n\n************************************************************************\n"); 
	fprintf(fp, "this is a new log\n\n");	// 
    fclose(fp);								// close file
}

//add str to debug.log file for easy debugging
int log(char *str){
	FILE *fp = fopen("debug.log", "a");		// fopen a FILE stream for APPEND
    fprintf(fp, "%s\n",str);				// append str to file
    fclose(fp);								// close FILE stream when done
}

//log argc, argv, and env
int logArgEnv(int argc, char *argv[], char *env[ ]){
	char strOut[128];
	sprintf(strOut, "argc = %d", argc);
	log(strOut);
	int i = 0;
	while(argv[i]){ 
		sprintf(strOut, "argv[%d] = %s", i, argv[i]); 
		log(strOut);
		i++; 
	}
	i = 0; 
	while(env[i]){ 
		sprintf(strOut, "env[%d] = %s", i, env[i]); 
		log (strOut);
		i++; 
	}	 
}