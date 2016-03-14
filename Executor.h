#ifndef Executor_H
#define Executor_H

//#define STDOUT_FILENO 1

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>

#include "Printer.h"

using namespace std;

class Executor{

	public:
		Executor(Printer* printer);
		virtual ~Executor();
		void evaluateCommand(string command);
		
	
	private:

		Printer* printer;
		//this is what strtok reads
		char* commandTokenization;
		int historyPosition;

		void redirectProgramToFile(string source, string destination);		
		void redirectFileToProgram(string source, string destination);
		void createPipe(string source, string destination);
		void executeProgram(string firstToken);
		void updateHistory(string command);
		void getHistory(char direction);
		void executePipe(int* pipeFDs, string source, string destination);
	
		//a list of the last 10 entered commands
		std::string history[10];
};

#endif //Executor_H