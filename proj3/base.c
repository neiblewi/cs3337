//Wang, K.C..Systems Programming in Unix / Linux(Kindle Locations 6525 - 6535).Springer International Publishing.Kindle Edition.
//Part 1: Project Base Code 
/******* Base Code of Programming Project *******/ 
#include <stdio.h> 
#include <signal.h> 
#include <fcntl.h> 
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define LEN 128
int pid; // child pid 
int downPipe[2]; // pipe descriptors 
int upPipe[2];	// pipe descriptors
char line[LEN]; 
time_t msgSend, msgReceive, replySend, replyReceive;

void toUpper(char* line) {
	for (int i = 0; i < strlen(line); i++) {
		if (line[i] >= (char)97 && line[i] <= (char)122)
			line[i] = line[i] - 32;
	}
}

void thandler(int sig) {
	printf("parent %d timer handler: got timer", getpid());
}

void phandler(int sig) {
	printf("parent %d got an interrupt sig =%d\n", getpid(), sig);
	read(upPipe[0], line, LEN); // read pipe 
	replyReceive = time(NULL);
	char temp[LEN];
	sprintf(temp, "\treplyReceive timestamp =%ld\tround trip =%ld sec", replyReceive, replyReceive-msgSend);
	strcat(line, temp);
	printf("parent %d got message = %s\n", getpid(), line);
}

void chandler(int sig) { 
	printf("child %d got an interrupt sig =%d\n", getpid(), sig); 
	read(downPipe[0], line, LEN); // read pipe 
	msgReceive = time(NULL);
	toUpper(line);
	char temp[LEN];
	sprintf(temp, "\tmsgReceive timestamp =%ld", msgReceive);
	strcat(line, temp);
	printf("child %d got message = %s\n", getpid(), line); 
	printf("child %d changed message = %s\n", getpid(), line);
	
	for (int i = 0; i < 123456789; i++) {} //simulate some time

	replySend = time(NULL);
	sprintf(temp, "\treplySend timestamp =%ld", replySend);
	strcat(line, temp);
	printf("child %d write to pipe\n", getpid());
	write(upPipe[1], line, LEN); // write to pipe 
	printf("child %d send signal 12 to %d\n", getpid(), getppid() );
	kill(getppid(), SIGUSR2); // send signal parent process 
} 

int parent() { 
	printf("parent %d running\n", getpid()); 
	close(downPipe[0]); // parent = pipe writer 
	signal(SIGUSR2, phandler); // install signal catcher
	signal(SIGVTALRM, thandler);
	close(upPipe[1]); // parent = pipe reader
	struct itimerval timer;
	while(1){ 
		printf("parent %d: input a line : \n", getpid());
		fgets(line, LEN, stdin); 
		line[strlen(line) - 1] = 0; // kill \n at end 
		msgSend = time(NULL);
		char temp[LEN];
		sprintf(temp, "\tmsgSend timestamp =%ld", msgSend);
		strcat(line, temp);
		printf("parent %d write to pipe\n", getpid()); 
		write(downPipe[1], line, LEN); // write to pipe 
		printf("parent %d send signal 10 to %d\n", getpid(), pid); 
		timer.it_value.tv_sec = 2;
		timer.it_value.tv_sec = 0;
		setitimer(ITIMER_VIRTUAL, &timer, NULL);
		kill(pid, SIGUSR1); // send signal to child process 


	} 
} 

int child() { 
	char msg[LEN]; 
	int parent = getppid(); 
	printf("child %d running\n", getpid()); 
	close(downPipe[1]); // child is pipe reader 
	close(upPipe[0]); // child is pipe writer
	signal(SIGUSR1, chandler); // install signal catcher 
	while(1); 
} 

int main() {
	pipe(downPipe); // create a pipe 
	pipe(upPipe);	//create pipe
	pid = fork(); // fork a child process 
	if (pid) // parent 
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