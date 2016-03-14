#ifndef Token_H
#define Token_H

//#define STDOUT_FILENO 1

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>

#include "Printer.h"

using namespace std;

class Token{

	public:
		Token(command, type, file);
		virtual ~Token();
		
		//forks, reads from the in pipe, and writes to the out pipe
		void execute();
		void setInPipe();
		void setOutPipe();
		void setPrev();
		void setNext();
		
	private:

		Token* prev;
		Token* next;
		int* inPipe;
		int* outPipe;
		string command;
		string type;
		string inFile;
		string outFile;
	
		//a list of the last 10 entered commands
};

#endif //Token_H