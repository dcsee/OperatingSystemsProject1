#ifndef CharParser_H
#define CharParser_H

//#define STDOUT_FILENO 1

#include <iostream>
#include <string>
#include <cstdlib>

#include "Executor.h"
#include "Printer.h"

class CharParser{

	public:
		CharParser(Printer* p);
		virtual ~CharParser();
		
		
		void receiveChar(char rc);
		
		//replaces the current command with the empty string
		void clearCommand();
	
	private:
	
		//an object to do high-level parsing and command execution on the currentCommand
		Executor* executor;
		Printer* printer;
	
		//the most recent input received
		char receivedCharacter;
		std::string[] tokenizedInput;
		
		//the command currently being typed by the user; hasn't been entered, so isn't part of the history yet
		std::string currentCommand;
};

#endif //CHARPARSER_H