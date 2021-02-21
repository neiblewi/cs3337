/*Lewis Neibaur
*cs3337
*this program simulates a sh terminal
*created from instuctions in chapter 3.11 of 'Systems Programming in Unix/Linux' by K.C. Wang
*/
#include "shsim.h"
char logStrOut[2048]; 	//global variable for use in logdebug
int logTabs;			//global variable for use in logdebug
char logPath[256];		//global variable for use in logdebug

//main function
int main( int argc, char *argv[], char *env[ ]){
	logReset();							
	logDebug("\nmain function:", logTabs);	
	logArgEnv(argc, argv, env);	
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
			forkChild(argCount, argVector, env);	//fork child to execute other commands
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
		//never runs becuase executecommand exits child
		sprintf(logStrOut, "CHILD end");			
		logDebug(logStrOut, logTabs);
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
	logArgEnv(argCount, argVector, NULL);
	char cmd[128]; 							//string to hold cmd
	getcmd(cmd, argVector[0], env);			//get path to cmd to be executed	
	sprintf(logStrOut, "exec cmd:%s", cmd); 
	logDebug(logStrOut, logTabs);
	int r = execve( cmd, argVector, env);	//execute cmd with args from argVector
	// come to here only if execve() failed 
	sprintf(logStrOut, "execve() failed: r = %d", r); 
	logDebug(logStrOut, logTabs);
	exit(r);								//exit process and return to parent
	logTabs--;
}
//gets a path to command in arg0 from path in env
void getcmd(char *cmd, char* arg0, char **env){
	logTabs++;
	sprintf(logStrOut, "getcmd()"); 
	logDebug(logStrOut, logTabs);
	//get path from env
	char *envPath = getenv("PATH");
	sprintf(logStrOut, "envPath = %s", envPath); 
	logDebug(logStrOut, logTabs);

	//get envpathcount
	int envPathCount = 1;							//count how many directories are in path. need at least one
	for(int i = 0; i < strlen(envPath); i++){		//loop throught input char by char
		if(envPath[i] == (int)':')					//find ':'
			envPathCount++;							//count how many ':' are in user input line
	}	
	sprintf(logStrOut, "pathCount= %i", envPathCount); 
	logDebug(logStrOut, logTabs);

	//split path by ':' to get directories
	char **envPaths = (char **)malloc((envPathCount + 1) * sizeof(char *)); //create new string array
	char *token = strtok(envPath, ":");			    //get first token
	int i = 0; 
	while(i < envPathCount){					//loop through tokens and array
		envPaths[i] = token;					//store token
		token = strtok(0, ":");					//go to next token
		i++;									//go to next index in array
	}
	envPaths[envPathCount] = NULL;				//array is null terminated




/*	sprintf(strOut, "envpaths(%p)", envPaths); 
	logDebug(strOut, tabs);
	getEnvPaths(&envPaths, envPath);
	sprintf(strOut, "envpaths(%p)", envPaths); 
	logDebug(strOut, tabs);
*/	//nothing after this line is running? seg fault in child process ends process
/*	i = 0;
	while(envPaths[i]){ 
		sprintf(strOut, "envPaths[%d] = %s", i, *envPaths[i]); 
		logDebug(strOut, tabs);
		i++; 
	}
*/	//search directories for arg0


	// piece together final cmd
	strcpy(cmd, "/bin/"); // create /bin/ command 
	strcat(cmd, arg0); 
	logTabs--;
}

void getEnvPaths(char ***envPaths, char *envPath){
	logTabs++;
	sprintf(logStrOut, "envpaths(%p) -> *envpaths(%p)", envPaths, *envPaths); 
	logDebug(logStrOut, logTabs);
	//count number of directories
	int envPathCount = 1;							//count how many directories are in path. need at least one
	for(int i = 0; i < strlen(envPath); i++){		//loop throught input char by char
		if(envPath[i] == (int)':')					//find ':'
			envPathCount++;							//count how many ':' are in user input line
	}	
	sprintf(logStrOut, "pathCount= %i", envPathCount); 
	logDebug(logStrOut, logTabs);
	
	// store directories in envPaths
	char ** newStrArray = (char **)malloc((envPathCount + 1) * sizeof(char *)); //create new string array
	sprintf(logStrOut, "newStrArray(%p)", newStrArray); 
	logDebug(logStrOut, logTabs);
	char *token;								//string to hold tokens
	token = strtok(envPath, ":");			    //get first token
	int i = 0; 
	while(i < envPathCount){					//loop through tokens and array
		newStrArray[i] = token;					//store token
		sprintf(logStrOut, "(%p)newStrArray[%i] = %s",newStrArray[i], i, token); 
		logDebug(logStrOut, logTabs);
		token = strtok(0, ":");					//go to next token
		i++;									//go to next index in array
	}
	newStrArray[envPathCount] = NULL;			//array is null terminated
	sprintf(logStrOut, "*envpaths(%p) = newStrArray(%p)", *envPaths, newStrArray); 
	logDebug(logStrOut, logTabs);
	*envPaths = (char **)newStrArray[0];					//change argVector to point to new array
	sprintf(logStrOut, "envpaths(%p) -> *envpaths(%p)", envPaths, *envPaths); 
	logDebug(logStrOut, logTabs);
	logTabs--;
	return;
}

/***********************functions for getting user input***************************/
//get a line of input from user and store in argCount and argVector
//input should be formatted as cmd arg1 arg2 arg3 .... argn
void getInput( char *line, int *argCount, char ***argVector){     //get a line of input from user
	logTabs ++;
	logDebug("getInput()", logTabs);
	getInputLine(line);							//get user input
	setArgcSim(line, argCount);					//count number of arguments
	strSplit(line, argCount, argVector, ' ');		//store arguments in arrPtr
	logTabs --;
}
//get line of input from user
void getInputLine(char *line){
	logTabs ++;
	char msg[128];
	getcwd(msg, 128);
	printf("%s:%s $ ", getlogin(), msg );       //display message to user
    fgets(line, 128, stdin);                    //get input line from user
	line[strlen(line)-1] = 0;                   //kill \n at the end of line
	sprintf(logStrOut, "got input: %s", line); 
	logDebug(logStrOut, logTabs);
	logTabs --;
}
//count the number of arguments from line
void setArgcSim(char *line, int *argCount){
	logTabs++;
	*argCount = 1;								//count how many args are in input. need n+1
	for(int i = 0; i < strlen(line); i++){		//loop throught input char by char
		if(line[i] == (int)' '){ 				//find ' '
			*argCount = *argCount + 1;				//count how many ' ' are in user input line
		}
	}	
	sprintf(logStrOut, "argCount= %i", *argCount); 
	logDebug(logStrOut, logTabs);
	logTabs --;
}
//tokenize and store arguments from inputStr
void strSplit(char *inputStr, int *arrLength, char ***arrPtr, char delimiter){
	logTabs++;
	char ** newStrArray = (char **)malloc((*arrLength + 1) * sizeof(char *)); //create new string array
	char *token;								//string to hold tokens
	token = strtok(inputStr, &delimiter);					//get first token
	int i = 0; 
	while(i < *arrLength){						//loop through tokens and array
		newStrArray[i] = token;					//store token
		i++;									//go to next index in array
		token = strtok(0, &delimiter);					//go to next token
	}
	newStrArray[*arrLength] = NULL;				//last arg points to null
	*arrPtr = newStrArray;					//change arrPtr to point to new array
	logArgEnv(*arrLength, *arrPtr, NULL);
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
	FILE *fp = fopen(logPath, "a");		// fopen a FILE stream for APPEND
	for (int i = 0; i < tabs; i++){
		fprintf(fp, "\t");
	}
	fprintf(fp, "%s\n",str);				// append str to file
	fclose(fp);								// close FILE stream when done
}
//log argc, argv, and env
void logArgEnv(int argc, char *argv[], char *env[ ]){
	logDebug("logArgEnv()--------------", logTabs);
	if (argc){
		sprintf(logStrOut, "argc = %d", argc);
		logDebug(logStrOut, logTabs);
	}
	if (argv){
		sprintf(logStrOut, "argvPtr = %p", argv);
		logDebug(logStrOut, logTabs);
	}
	int i = 0;
	while(argv && argv[i]){ 
		sprintf(logStrOut, "argv[%d] = %s", i, argv[i]); 
		logDebug(logStrOut, logTabs);
		i++; 
	}
	if (argv && argc){
		sprintf(logStrOut, "argv[%d] = %s", argc, argv[argc]);
		logDebug(logStrOut, logTabs);
	}
	i = 0; 
	while(env && env[i]){
		sprintf(logStrOut, "env[%d]=%s ", i, env[i]);
		logDebug (logStrOut, logTabs);
		i++;
	}
}

void logArray(char** arr, int arrLength) {
	sprintf(logStrOut, "strArray()");
	logDebug(logStrOut, logTabs);
	if (arrLength) {
		sprintf(logStrOut, "length=%i", arrLength);
		logDebug(logStrOut, logTabs);
	}
	if (arr) {
		sprintf(logStrOut, "arrayPtr=%p", arr);

		logDebug(logStrOut, logTabs);
		int i = 0;
		while (arr[i])
		{
			sprintf(logStrOut, "array[%i]=%s", i, arr[i]);
			logDebug(logStrOut, logTabs);
			i++;
		}
	}
}