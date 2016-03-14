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

	//pipefd[0] is the read end of the pipe,
	//pipefd[1] is the write end of the pipe
	//returns 0 on success
		
	//If the parent wants to receive data from the child, it should close fd1, and the child should close fd0. 
	//If the parent wants to send data to the child, it should close fd0, and the child should close fd1
	
	int childStatus;
	int dupstatus;
	int pipeFDs[2];
	pipe(pipeFDs);
	
	pid_t forkedProcess = fork();
	
	if(forkedProcess == 0){
		pid_t fork2 = fork();
		
		if(fork2 == 0){
			//this is wc in ls | wc
			dup2(pipeFDs[0], STDIN_FILENO);	//duplicate the pipe to stdin
			close(pipeFDs[1]);	//close the write end
			//char buffer[7];
			//read(STDIN_FILENO, buffer, 6);
			//write(STDOUT_FILENO, buffer, 6);
			//const char* command = "wc";
			//execlp(command, command, 0);	//execute the command
			execlp(destination.c_str(), destination.c_str(), 0);	//execute the command
			exit(0);
		}
		else{
			//back in ls
			dup2(pipeFDs[1], STDOUT_FILENO); //duplicate pipe to stdout
			close(pipeFDs[0]); //close the read end
			//char* buffer = "hello!";
			//write(STDOUT_FILENO, buffer, 6);
			//const char* command = "ls";
			execlp(source.c_str(), source.c_str(), 0);	//execute command
			exit(0);
		}
	}
	else{
		int status;
		close(pipeFDs[0]);
		close(pipeFDs[1]);
		waitpid(forkedProcess, &status, 0);	//wait for child process to finish
	}
}