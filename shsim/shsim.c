/*Lewis Neibaur
*cs3337
*this program simulates a sh terminal
*created from instuctions in chapter 3.11 of 'Systems Programming in Unix/Linux' by K.C. Wang
*/
#include "shsim.h"
char strOut[2048]; 	//global variable for use in logdebug
int tabs;			//global variable for use in logdebug
char logPath[256];	//global variable for use in logdebug

//main function
int main( int argc, char *argv[], char *env[ ]){
	logReset();							
	logDebug("\nmain function:", tabs);	
	logArgEnv(argc, argv, env);	
	int i = 1;							
	while(i){									//main program loop
		int argCount;							//empty integer to simulate argc
		char **argVector = NULL;				//empty pointer to array of strings to simulate argv
		char line[256];							//string to hold user input line
		logDebug("getting input", tabs);
		getInput(line, &argCount, &argVector);	//get input from user
		if( argVector[0] && !strcmp(argVector[0],"exit")){		//if first argument is "exit"
			sprintf(strOut, "exiting process: pid=%d", getpid()); 
			logDebug(strOut, tabs);
			exit(0);							//exit program
		}
		else if (argVector[0] && !strcmp(argVector[0],"cd")){	//if first argument is "cd"	
			logDebug("changing directory", tabs);
			cd(argVector[1]);					//change directory
		}
		else{									//for all other commands
			logDebug("other command", tabs);
			forkChild(argCount, argVector, env);	//fork child to execute other commands
		}
		free(argVector);						//dealocate memory from argVector array before repeating loop
	}	
}

/****************************************function definitions*******************************************/
//changes cwd to path
void cd(char *path){
	tabs++;
	int error;
	if (path)							//if there is a seccond argument
		error = chdir(path);			//try to change to path in 2nd arg
	else								//if 2nd arg was null
		error = chdir(getenv("HOME"));	//try to change cwd to home
	if (error)							//if cd was not successful
		printf("error: invalid directory path: %s\n", path);	// display error to user
	char newPath[128];
	getcwd(newPath, 128);
	sprintf(strOut, "cwd changed to: %s", newPath);
	logDebug(strOut, tabs); 
	tabs--;
}
//fork a child process and wait for it to finish
void forkChild(int argCount, char **argVector, char **env){
	tabs++;
	sprintf(strOut, "FORK FROM: pid=%d	ppid=%d", getpid(), getppid());
	logDebug(strOut, tabs); 
	int pid, status; 
	pid = fork(); 									// fork syscall; parent returns child pid, 
	if (pid){ 										// PARENT EXECUTES THIS PART 
		sprintf(strOut, "PARENT: pid=%d	pidChild=%d", getpid(), pid);
		logDebug(strOut, tabs); 
		pid = wait(&status); 						// wait for ZOMBIE child process
		sprintf(strOut, "PARENT end");
		logDebug(strOut, tabs); 
	}
	else{ 											// CHILD executes this part 
		sprintf(strOut, "CHILD: pid=%d	ppid=%d", getpid(), getppid());
		logDebug(strOut, tabs);
		executeCommand(argCount, argVector, env);	//child executes command in argvector
		//never runs becuase executecommand exits child
		sprintf(strOut, "CHILD end");			
		logDebug(strOut, tabs);
	}
	sprintf(strOut, "AFTER FORK: pid=%d	ppid=%d", getpid(), getppid());
	logDebug(strOut, tabs); 
	tabs--;
}
//change proccess image to command specified
void executeCommand(int argCount, char **argVector, char **env){
	tabs++;
	sprintf(strOut, "pid=%d	ppid=%d", getpid(), getppid()); 
	logDebug(strOut, tabs);
	logArgEnv(argCount, argVector, NULL);
	char cmd[128]; 							//string to hold cmd
	getcmd(cmd, argVector[0], env);			//get path to cmd to be executed	
	sprintf(strOut, "exec cmd:%s", cmd); 
	logDebug(strOut, tabs);
	int r = execve( cmd, argVector, env);	//execute cmd with args from argVector
	// come to here only if execve() failed 
	sprintf(strOut, "execve() failed: r = %d", r); 
	logDebug(strOut, tabs);
	exit(r);								//exit process and return to parent
	tabs--;
}
//gets a path to command in arg0 from path in env
void getcmd(char *cmd, char* arg0, char **env){
	tabs++;
	//get path from env
	char *envPath = getenv("PATH");
	sprintf(strOut, "envPath = %s", envPath); 
	logDebug(strOut, tabs);

	//split path by ':' to get directories
	char ** envPaths;
	getEnvPaths(&envPaths, envPath);
	//nothing after this line is running? seg fault in child process ends process
/*	int i = 0;
	while(envPaths[i]){ 
		sprintf(strOut, "envPaths[%d] = %s", i, *envPaths[i]); 
		logDebug(strOut, tabs);
		i++; 
	}
*/	//search directories for arg0


	// piece together final cmd
	strcpy(cmd, "/bin/"); // create /bin/ command 
	strcat(cmd, arg0); 
	tabs--;
}

void getEnvPaths(char ***envPaths, char *envPath){
	tabs++;
	//count number of directories
	int envPathCount = 1;							//count how many directories are in path. need at least one
	for(int i = 0; i < strlen(envPath); i++){		//loop throught input char by char
		if(envPath[i] == (int)':')					//find ':'
			envPathCount++;							//count how many ':' are in user input line
	}	
	sprintf(strOut, "pathCount= %i", envPathCount); 
	logDebug(strOut, tabs);
	
	// store directories in envPaths
	char ** newStrArray = (char **)malloc((envPathCount + 1) * sizeof(char *)); //create new string array
	char *token;								//string to hold tokens
	token = strtok(envPath, ":");			    //get first token
	int i = 0; 
	while(i < envPathCount){					//loop through tokens and array
		newStrArray[i] = token;					//store token
		sprintf(strOut, "newStrArray[%i] = %s", i, token); 
		logDebug(strOut, tabs);
		token = strtok(0, ":");					//go to next token
		i++;									//go to next index in array
	}
	newStrArray[envPathCount] = NULL;			//array is null terminated
	*envPaths = newStrArray;					//change argVector to point to new array
	sprintf(strOut, "directory array filled"); 
	logDebug(strOut, tabs);
	tabs--;
	return;
}

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
	i = 0; 
	while(env && env[i]){
		sprintf(strOut, "env[%d]=%s ", i, env[i]);
		logDebug (strOut, tabs);
		i++;
	} 
}