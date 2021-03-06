//Wang, K.C..Systems Programming in Unix / Linux(Kindle Locations 6525 - 6535).Springer International Publishing.Kindle Edition.
//Part 1: Project Base Code 
/******* Base Code of Programming Project *******/ 
#include <stdio.h> 
#include <signal.h> 
#include <fcntl.h> 
#include <string.h>
#include <time.h>
#include <sys/time.h>

//global vars
#define LEN 128
int pid;			// child pid 
int downPipe[2];	// downpipe descriptors 
int upPipe[2];		// uppipe descriptors
char line[LEN];		// line to hold msg
time_t msgSend, msgReceive, replySend, replyReceive;	//timestamps
int rplyRecieved;	// 0 = no reply recieved. 1 = reply recieved
struct itimerval timer;		//timer for timeout

//change all aphabetic charcacters to uppercase.
void toUpper(char* line) {
	for (int i = 0; i < strlen(line); i++) {
		if (line[i] >= (char)97 && line[i] <= (char)122)
			line[i] = line[i] - 32;
	}
}

//handler for sig 26
void timer_handler(int sig) {
	printf("parent % d got an interrupt sig = % d\n", getpid(), sig);
	printf("parent %d send signal 10 to %d\n", getpid(), pid);
	kill(pid, SIGUSR1);					// send signal to child process
}

//handler for sig  12
void phandler(int sig) {
	printf("parent %d got an interrupt sig =%d\n", getpid(), sig);
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 0;			
	setitimer(ITIMER_VIRTUAL, &timer, NULL);	// cancel VIRTUAL itimer 
	//recieve reply
	rplyRecieved = 1;
	read(upPipe[0], line, LEN); // read pipe 
	replyReceive = time(NULL);	//get timestamp
	char temp[LEN];
	sprintf(temp, " | rRcvTS=%ld | rndTrip=%ld sec", replyReceive, replyReceive-msgSend);
	strcat(line, temp);			//add timestamp
	printf("parent %d got message = %s\n", getpid(), line);
}

//handler for sig 11
void chandler(int sig) { 
	printf("child %d got an interrupt sig =%d\n", getpid(), sig); 
	//get message
	read(downPipe[0], line, LEN);	// read pipe 
	msgReceive = time(NULL);		//get timestamp
	char temp[LEN];
	sprintf(temp, " | rcvTS=%ld", msgReceive);	//add timestamp
	strcat(line, temp);
	printf("child %d got message = %s\n", getpid(), line); 
	//change message
	toUpper(line);					//convert to UPPPERCASE
	printf("child %d changed message = %s\n", getpid(), line);
	//simulate delay(optional)
	for (int i = 0; i < 300000000; i++) {}		//simulate some time
	//prepare reply
	replySend = time(NULL);						//record time stamp
	sprintf(temp, " | rSendTS=%ld", replySend);	//add time stamp
	strcat(line, temp);
	//send reply
	printf("child %d write to pipe\n", getpid());
	write(upPipe[1], line, LEN);	// write to pipe 
	printf("child %d send signal 12 to %d\n", getpid(), getppid() );
	kill(getppid(), SIGUSR2);		// send signal parent process 
} 

// main function for parent process
int parent() { 
	printf("parent %d running\n", getpid()); 
	close(downPipe[0]);			// parent = downpipe writer 
	close(upPipe[1]);			// parent = uppipe reader
	signal(SIGUSR2, phandler);	// install signal catcher for signal from child
	signal(SIGVTALRM, timer_handler);	// install signal catcher for signal from timer
	while(1){ 
		printf("parent %d: input a line : \n", getpid());
		//get message
		fgets(line, LEN, stdin);			// line from user
		rplyRecieved = 0;
		//prepare message
		line[strlen(line) - 1] = 0;			// kill \n at end 
		msgSend = time(NULL);				//get first time stamp
		char temp[LEN];
		sprintf(temp, "\n\tsendTS=%ld", msgSend);		//add timestamp
		strcat(line, temp);
		//send message
		printf("parent %d write to pipe\n", getpid()); 
		write(downPipe[1], line, LEN);		// write to pipe 
		//signal child
		printf("parent %d send signal 10 to %d\n", getpid(), pid); 
		kill(pid, SIGUSR1);					// send signal to child process
		//check for reply. resignal on timeout
		while (!rplyRecieved) {
			//set timers
			timer.it_value.tv_sec = 0;
			timer.it_value.tv_usec = 100000;			// 10000 nsec = 10 msec
			setitimer(ITIMER_VIRTUAL, &timer, NULL);	// Start a VIRTUAL itimer 
			for (int i = 0; i < 100000000; i++) {}		//simulate some time
		} 
	}
} 

// main function for child process
int child() { 
	printf("child %d running\n", getpid()); 
	close(upPipe[0]);			// child is pipe writer
	close(downPipe[1]);			// child is pipe reader 
	signal(SIGUSR1, chandler);	// install signal catcher 
	while(1);					// continue running
} 

//main function for program
int main() {
	pipe(downPipe);		// create a pipe 
	pipe(upPipe);		//create pipe
	pid = fork();		// fork a child process 
	if (pid) 
		parent();
	else
		child();
}




/*(1).Send Reply :
	After sending a message the sender waits for a reply before sending the next message.
	Correspondingly, after receiving a message the receiver sends a reply back to the sender.
	For example, the reply may be the original message converted to uppercase. 
(2).Time Stamp : 
	When sending a message or reply, add a time stamp to the message by the current time.
	After receiving a reply, compute and show the message round trip time. 
(3).Timeoutand Resend : 
	Assume that the parent process is unreliable.After receiving a message, 
	it may decide randomly not to send a reply, which would cause the sender to wait indefinitely.
	Add timeoutand resend as follows.
	After sending a message, set a real - time mode interval timer of 10 msec.
	If received a reply before the timer expires, cancel the timer.Otherwise, 
	resend the message again(with a new time stamp) until a reply is received.
	*/