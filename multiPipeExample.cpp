#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>

using namespace std;

int main(){

	int numProcesses = 1;//for example
	int numPipes = numProcesses - 1;

	//pipe[0] is the read end of the pipe,
	//pipe[1] is the write end of the pipe
	//returns 0 on success
		
	//If the parent wants to receive data from the child, it should close fd1, and the child should close fd0. 
	//If the parent wants to send data to the child, it should close fd0, and the child should close fd1
	
	int** arrayOfPipes = new int*[numPipes];

	for(int i = 0; i < numPipes; i++){
		//this initializes all the pipes we will need
		arrayOfPipes[i] = new int[2];
		pipe(arrayOfPipes[i]);
	}

	for(int i = 0; i < numProcesses; i++){
		int pid = -1;
		
		if((pid = fork()) == 0){
			//create a new process to execute
			if(i == 0){
				//if this is the first process, it only has access to an outgoing pipe
				cout << "HELLO" << endl;
				
				dup2(arrayOfPipes[i][1], STDOUT_FILENO); //duplicate pipe to stdout
				close(arrayOfPipes[i][0]); //close the read end
				const char* command = "ls";
				execlp(command, command, 0);	//execute command
				exit(0);
			}
			else if(i == (numProcesses - 1)){
				//if this is the last process, it only has access to an incoming pipe
				dup2(arrayOfPipes[i - 1][0], STDIN_FILENO);	//duplicate incoming pipe read pipe to stdin
				close(arrayOfPipes[i - 1][1]);	//close incoming pipe write end
				const char* command = "wc";
				execlp(command, command, 0);	//execute command
				exit(0);
			}
			else{
				//we're between pipes, so we need to set up both incoming and outgoing pipes
				dup2(arrayOfPipes[i][1], STDOUT_FILENO); //duplicate pipe to stdout
				//close(arrayOfPipes[i][0]); //close the read end
				dup2(arrayOfPipes[i - 1][0], STDIN_FILENO);	//duplicate incoming pipe read pipe to stdin
				//close(arrayOfPipes[i - 1][1]);	//close incoming pipe write end
				const char* command = "wc";
				execlp(command, command, 0);	//execute command
				exit(0);
			}
		}
		
	}
	for(int i = 0; i < numPipes; i++){
		close(arrayOfPipes[i][0]);
		close(arrayOfPipes[i][1]);
	}
}