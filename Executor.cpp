#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "Executor.h"

#define NO_ERROR 0


using namespace std;

Executor::~Executor(){

}

Executor::Executor(Printer* p){
	historyPosition = 0;
	printer = p;
	for(int i = 0; i < 10; i++)
		history[i] = "";
}

void Executor::updateHistory(string command){

	if(history[0] == "")//the history is empty
		history[0] = command;
	else{
		for(int i = 0; i < 10; i++){
			string temp = history[i];
			history[i] = command;
			command = temp;
		}
	}
}

string Executor::getHistory(char direction){
	
	//if location is blank, don't move
	
	switch(direction){
		
		case 'u':
			if(direction )
			return 
		
	}
}

void Executor::executePipe(int* pipeFDs, string source, string destination){

	pid_t fork2 = fork();

	if(fork2 == 0){
		//this is wc in ls | wc
		dup2(pipeFDs[0], STDIN_FILENO);	//duplicate the pipe to stdin
		close(pipeFDs[1]);	//close the write end
		execlp(destination.c_str(), destination.c_str(), 0);	//execute the command
		exit(0);
	}
	else{
		//back in ls
		dup2(pipeFDs[1], STDOUT_FILENO); //duplicate pipe to stdout
		close(pipeFDs[0]); //close the read end
		execlp(source.c_str(), source.c_str(), 0);	//execute command
		exit(0);
	}
}

void Executor::createPipe(string source, string destination){
	
	int childStatus;
	int dupstatus;
	int incoming[2];
	int outgoing[2];
	pipe(incoming);
	pipe(outgoing);
	
	pid_t forkedProcess = fork();
	
	if(forkedProcess == 0){
		executePipe(pipeFDs, source, destination);
	}
	else{
		int status;
		close(pipeFDs[0]);
		close(pipeFDs[1]);
		waitpid(forkedProcess, &status, 0);	//wait for child process to finish
	}
	
	//checking for more in the loop
	char* token;
	char* nextProcess;
	string tokenString = "";
	while((token = strtok(NULL, " ")) != NULL){
		if(string(token) == "|"){		//if pipe, call pipe()
			pipe(pipeFDs[2]);
			forkedProcess = fork();
			if(forkedProcess == 0){
				//make the process write to the pipe
				dup2(pipeFDs[1], STDOUT_FILENO); //duplicate pipe to stdout
				close(pipeFDs[0]); //close the read end
				execlp(source.c_str(), source.c_str(), 0);	//execute command
				exit(0);
			}
		
		
		nextProcess = strtok(NULL, " ");
		if(nextProcess = NULL)	//nowhere to pipe to
			break;
		else{
			destination = string(nextProcess);
		}
		//if > or <, call redirect
		}
	}

}

void Executor::redirectFileToProgram(string program, string file){
	
//	STDOUT_FILENO
//	STDIN_FILENO
	
	//testing the case of wc < file.txt, where wc is an external command to be executed, and
	//file.txt is a text file
	
	//file - the one that provides the STDOUT
	//program - the one that receives the STDIN
	//redirects file's STDOUT to program's STDIN
	
	//1. fork
	pid_t forkedProcess = fork();

	//2. open file file descriptor with O_RDONLY
	int sourceFileDescriptor = open(file.c_str(), O_RDONLY);	

	//3. for the forked process, dup2 program's stdin to be the file descriptor of the file
	if(forkedProcess == 0){
		dup2(sourceFileDescriptor, STDIN_FILENO);
		char* token = strtok(NULL, " ");
		if(token != NULL){
			executeProgram(string(token));
		}
		execlp(program.c_str(), program.c_str(), 0);		//execute the command
		exit(0);
	}
	else{
		//this is what the parent process executes
		int childStatus = 0;
		printer->prettyPrint("", false);
		waitpid(forkedProcess, &childStatus, 0);
		close(sourceFileDescriptor);
	}

	//command < input-file > output-file
	//redirects input-file to command's stdin
	//then redirects command's stdout to output-file
	
	//< input-file command > output-file
	//does the same thing.
	//redirects input-file to command's stdin,
	//then redirects command's stdout to output-file
	
	//> redirects program stdout to a file
	//< redirects a file to a program's stdin
	
}

void Executor::redirectProgramToFile(string program, string file){
	
	//1. fork
	pid_t forkedProcess = fork();

	//2. open file file descriptor with O_RDONLY
	int sourceFileDescriptor = open(file.c_str(), (O_CREAT | O_WRONLY));	

	//3. for the forked process, dup2 program's stdin to be the file descriptor of the file
	if(forkedProcess == 0){
		dup2(sourceFileDescriptor, STDOUT_FILENO);
		//here, check for more arguments in the command string
		char* token = strtok(NULL, " ");
		if(token != NULL){
			executeProgram(string(token));
		}
		execlp(program.c_str(), program.c_str(), 0);		//execute the command
		exit(0);
	}
	else{
		//this is what the parent process executes
		int childStatus = 0;
		printer->prettyPrint("", false);
		waitpid(forkedProcess, &childStatus, 0);
		close(sourceFileDescriptor);
	}
}

void Executor::executeProgram(string firstToken){
	//firstToken is the first term of the command, eg "cd" in "cd home"
	//first, check to see if the next character is a pipe or redirect
	
	string secondToken = "";
	string thirdToken = "";
	
	//get the next two tokens because we'll need them in all cases
	char* token = strtok(NULL, " ");
	if(token != NULL)
		secondToken = string(token);
	token = strtok(NULL, " ");
	if(token != NULL)
		thirdToken = string(token);
	
	if(secondToken == "|"){
		if(thirdToken == ""){
			printer->prettyPrint(string("pipe error - your pipe is missing a link"), false);
			return;
		}
		//using "this" just in case createPipe() is also a system call
		this->createPipe(firstToken, thirdToken);
	}
	else if(firstToken == "<"){	//means the file comes next
		if((secondToken != "") && (thirdToken != "")){//if the other two arguments are present,
		//the second must be the file, and the third must be the program
			redirectFileToProgram(thirdToken, secondToken);
			//redirect will execute; we don't necessarily want that
		}
	}
	else if(firstToken == ">"){
		if((secondToken != "") && (thirdToken != "")){//if the other two arguments are present,
		//the second must be the file, and the third must be the program
			redirectProgramToFile(thirdToken, secondToken);
			//redirect will execute; we don't necessarily want that
		}//done checking for the first token
	}
	else if(secondToken == ">"){
		//get the third argument and redirect it
		//redirecting A > B
		if(thirdToken == ""){
			//there was no other token, so you can't redirect to anything
			printer->prettyPrint(string("redirect error - no destination for redirect"), false);
			return;
		}
		redirectProgramToFile(firstToken, thirdToken);
	}
	else if(secondToken == "<"){
		//redirecting A < B	
		if(thirdToken == " "){
			printer->prettyPrint(string("redirect error - no source for redirect"), false);
			return;
		}
		//testing the case of wc < file.txt
		redirectFileToProgram(firstToken, thirdToken);
		return;
	}//done checking all arrows, time to check for pipes
	else{
		//secondToken is null
		//the user must be executing an external command, so attempt to call exec on it
		//printer->prettyPrint(string("no pipes or redirects found, forking"), false);

		pid_t forkedProcess = fork();
		if(forkedProcess == 0){
			//this one is the forked process
			int childStatus = execlp(firstToken.c_str(), firstToken.c_str(), 0);		
			exit(0);
		}
		else{
			//this is what the parent process executes
			int childStatus = 0;
			waitpid(forkedProcess, &childStatus, 0);
		}
	}
}

void Executor::tokenize(){
	
}

void Executor::evaluateCommand(string command){
	
	if(command.length() == 0){
		//no command to evaluate
		return;
	}
	
	updateHistory();
	
	string token = getNextToken();
	
	commandTokenization = new char[command.length() + 1];	
	strcpy(commandTokenization, command.c_str());
	//strtok relies on a CLASS VARIABLE, so all other calls to strtok must be called with null
	char* token = strtok(commandTokenization, " ");
	string stringToken = string(token);
	
	while(token != NULL){	

		if(stringToken == "cd"){
			printer->prettyPrint(stringToken, false);
		}
//		else if(stringToken == "ls"){
//			printer->prettyPrint(stringToken, false);
//		}
		else if(stringToken == "pwd"){
			printer->prettyPrint(stringToken, false);
		}
		else if(stringToken == "history"){
			printer->prettyPrint(stringToken, false);
		}
		else if(stringToken == "exit"){
			exit(NO_ERROR);
		}
		else{
			//if the token is not one of the recognized commands, then the user must be trying to execute a command
			//executeProgram(stringToken);
		}
		
		token = strtok(NULL, " ");
		if(token != NULL)
			stringToken = string(token);
	}
}