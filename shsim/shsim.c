/*Lewis Neibaur
*cs3337
*this program simulates a sh terminal
*created from instuctions in chapter 3.11 of 'Systems Programming in Unix/Linux' by K.C. Wang
*/
#include "shsim.h"

//main function
int main( int argc, char *argv[], char *env[ ]){
	printf("argc = %d \n", argc); 
	int i; 
	i = 0; 
	while(argv[i]){ 
		printf("argv[%d] = %s \n", i, argv[i]); 
		i++; 
	}
	i = 0; 
	while(env[i]){ 
		printf("env[%d] = %s \n", i, env[i]); 
		i++; 
	}	 
}