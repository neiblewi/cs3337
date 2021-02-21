/*Lewis Neibaur
*cs3337
*this program simulates a sh terminal
*created from instuctions in chapter 3.11 of 'Systems Programming in Unix/Linux' by K.C. Wang
*/
#include "shsim.h"
//global variables for use in logdebug
char logStrOut[2048], logPath[256];
int logTabs;

//main function
int main( int argc, char *argv[], char *env[ ]){
	logReset();							
	logDebug("\nmain function:", logTabs);	
	logStrArray(argc, argv, "main(argc,argv)");
	logStrArray(0, env, "main(env)");
	int i = 1;							
	while(i){									//main program loop
		int argCount;							//empty integer to simulate argc
		char **argVector = NULL;				//empty pointer to array of strings to simulate argv
		char line[256];							//string to hold user input line
		logDebug("getting input", logTabs);
		getInput(line, &argCount, &argVector);	//get input from user
		if( argVector[0] && !strcmp(argVector[0],"exit")){		//if first argument is "exit"
			sprintf(logStrOut, "exiting process: pid=%d", getpid()); 
			logDebug(logStrOut, logTabs);
			exit(0);							//exit program
		}
		else if (argVector[0] && !strcmp(argVector[0],"cd")){	//if first argument is "cd"	
			logDebug("changing directory", logTabs);
			cd(argVector[1]);					//change directory
		}
		else{									//for all other commands
			logDebug("other command", logTabs);
			forkChild(argCount, argVector, env);//fork child to execute other commands
		}
		free(argVector);						//dealocate memory from argVector array before repeating loop
	}	
}

/****************************************function definitions*******************************************/
//changes cwd to path
void cd(char *path){
	logTabs++;
	int error;
	if (path)							//if there is a seccond argument
		error = chdir(path);			//try to change to path in 2nd arg
	else								//if 2nd arg was null
		error = chdir(getenv("HOME"));	//try to change cwd to home
	if (error)							//if cd was not successful
		printf("error: invalid directory path: %s\n", path);	// display error to user
	char newPath[128];
	getcwd(newPath, 128);
	sprintf(logStrOut, "cwd changed to: %s", newPath);
	logDebug(logStrOut, logTabs); 
	logTabs--;
}

//fork a child process and wait for it to finish
void forkChild(int argCount, char **argVector, char **env){
	logTabs++;
	sprintf(logStrOut, "FORK FROM: pid=%d	ppid=%d", getpid(), getppid());
	logDebug(logStrOut, logTabs); 
	int pid, status; 
	pid = fork(); 									// fork syscall; parent returns child pid, 
	if (pid){ 										// PARENT EXECUTES THIS PART 
		sprintf(logStrOut, "PARENT: pid=%d	pidChild=%d", getpid(), pid);
		logDebug(logStrOut, logTabs); 
		pid = wait(&status); 						// wait for ZOMBIE child process
		sprintf(logStrOut, "PARENT end");
		logDebug(logStrOut, logTabs); 
	}
	else{ 											// CHILD executes this part 
		sprintf(logStrOut, "CHILD: pid=%d	ppid=%d", getpid(), getppid());
		logDebug(logStrOut, logTabs);
		executeCommand(argCount, argVector, env);	//child executes command in argvector
	}
	sprintf(logStrOut, "AFTER FORK: pid=%d	ppid=%d", getpid(), getppid());
	logDebug(logStrOut, logTabs); 
	logTabs--;
}

//change proccess image to command specified
void executeCommand(int argCount, char **argVector, char **env){
	logTabs++;
	sprintf(logStrOut, "pid=%d	ppid=%d", getpid(), getppid()); 
	logDebug(logStrOut, logTabs);
	logStrArray(argCount, argVector, "argVector");
	
	char* envPath = getenv("PATH");					//get PATH from env
	sprintf(logStrOut, "envPath = %s", envPath);
	logDebug(logStrOut, logTabs);
	
	int envPathCount;								//length of array
	strArrCount(envPath, &envPathCount, ':');		//get length of array
/*
	envPathCount = 1;								//count how many directories are in path. need at least one
	for (i = 0; i < strlen(envPath); i++) {			//loop throught input char by char
		if (envPath[i] == (int)':')					//find ':'
			envPathCount++;							//count how many ':' are in user input line
	}
	sprintf(logStrOut, "pathCount= %i", envPathCount);
	logDebug(logStrOut, logTabs);
*/
	char** envDirPaths = NULL;						//array of strings to hold directory paths
	strSplit(envPath, &envPathCount, &envDirPaths, ':');//fill array
	
//	i = 0;
	int r, i = 0;
	char cmd[256]; 									//string to hold cmd
	while (envDirPaths[i]) {						//loop through all directories
		strcpy(cmd, envDirPaths[i]);				//  /dir		command path
		strcat(cmd, "/");							//  /dir/		command path
		strcat(cmd, argVector[0]);					//  /dir/cmd	command path
		sprintf(logStrOut, "CHILD END: exec cmd:%s", cmd);
		logDebug(logStrOut, logTabs);
		r = execve( cmd, argVector, env);			//attempt to execute cmd with args from argVector
		// come to here only if execve() failed
		i++;
	}
	sprintf(logStrOut, "execve() failed: r = %d", r); 
	logDebug(logStrOut, logTabs);
	exit(r);								//exit process and return to parent
	logTabs--;
}


/***********************functions for getting user input***************************/
//get a line of input from user and store in argCount and argVector. input should be formatted as cmd arg1 arg2 arg3 .... argn
void getInput( char *line, int *argCount, char ***argVector){  
	logTabs ++;
	logDebug("getInput()", logTabs);
	getInputLine(line);								//get user input
	strArrCount(line, argCount, ' ');				//count number of arguments
	strSplit(line, argCount, argVector, ' ');		//store arguments in arrPtr
	logTabs --;
}

//get line of input from user
void getInputLine(char *line){
	logTabs ++;
	char msg[128];								
	getcwd(msg, 128);							//get cwd to display to user
	printf("%s:%s $ ", getlogin(), msg );       //display message to user
    fgets(line, 128, stdin);                    //get input line from user
	line[strlen(line)-1] = 0;                   //kill \n at the end of line
	sprintf(logStrOut, "got input: %s", line); 
	logDebug(logStrOut, logTabs);
	logTabs --;
}

//count the number of arguments from line and store in argCount
void strArrCount(char *line, int *argCount, char delimiter){
	logTabs++;
	*argCount = 1;								//count how many args are in input. need n+1
	for(int i = 0; i < strlen(line); i++){		//loop throught input char by char
		if(line[i] == (int)delimiter){ 			//find delimetr
			*argCount = *argCount + 1;			//count how many delimiter are in user input line
		}
	}	
	sprintf(logStrOut, "argCount= %i", *argCount); 
	logDebug(logStrOut, logTabs);
	logTabs --;
}

//tokenize values from inputStr and stoe in arrptr
void strSplit(char *inputStr, int *arrLength, char ***arrPtr, char delimiter){
	logTabs++;
	char ** newStrArray = (char **)malloc((*arrLength + 1) * sizeof(char *)); //create new string array
	char *token;								//string to hold tokens
	token = strtok(inputStr, &delimiter);		//get first token
	int i = 0; 
	while(i < *arrLength){						//loop through tokens and array
		newStrArray[i] = token;					//store token
		i++;									//go to next index in array
		token = strtok(0, &delimiter);			//go to next token
	}
	newStrArray[*arrLength] = NULL;				//last arg points to null
	*arrPtr = newStrArray;						//change arrPtr to point to new array
	logStrArray(*arrLength, *arrPtr, "strSplit()");
	logTabs --;
}

/******************functions for writing to debug.log file***********************/

//reset debug.log file
void logReset(){
	logTabs = 0;
	getcwd(logPath, 128);
	strcat(logPath, "/debug.log");
	FILE *fp = fopen(logPath, "w");		// fopen a FILE stream for 
    fprintf(fp, "****new log****\n"); 	// add message
    fclose(fp);
}

//add str to debug.log file for easy debugging
void logDebug(char *str, int tabs){
	FILE *fp = fopen(logPath, "a");			// fopen a FILE stream for APPEND
	for (int i = 0; i < tabs; i++){			// tabulate log to easily see what scope things happen in
		fprintf(fp, "\t");					//	add tabs to line in file
	}
	fprintf(fp, "%s\n",str);				// append str to file
	fclose(fp);								// close FILE stream when done
}

//log arrLength, strArray
void logStrArray(int arrLength, char *strArray[], char *title){
	sprintf(logStrOut, "logStrArray(%s)", title);			//display title
	logDebug(logStrOut, logTabs);
	logTabs++;
	if (arrLength){
		sprintf(logStrOut, "arrLength = %d", arrLength);	//dislplay length
		logDebug(logStrOut, logTabs);
	}
	if (strArray){
		sprintf(logStrOut, "arrPtr = %p", strArray);		//display adress
		logDebug(logStrOut, logTabs);
	}
	int i = 0;
	while(strArray && strArray[i]){ 
		sprintf(logStrOut, "strArray[%d] = %s", i, strArray[i]);		//show each element of array
		logDebug(logStrOut, logTabs);
		i++; 
	}
	if (strArray && arrLength){
		sprintf(logStrOut, "strArray[%d] = %s", arrLength, strArray[arrLength]);	//show last element for null terminated arrays
		logDebug(logStrOut, logTabs);
	}
	logTabs--;
}

