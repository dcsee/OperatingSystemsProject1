#include <unistd.h>
#include "charParser.h"
#include "Executor.h"

#include <string>
using namespace std;

CharParser::~CharParser(){
	
}

CharParser::CharParser(Printer* p){

	printer = p;
	executor = new Executor(printer);
}

//replaces the current command with the empty string
void CharParser::clearCommand(){
	currentCommand = "";
}


//receives a single character from the input loop and appends it to the current command
void CharParser::receiveChar(char rc){
	
	receivedCharacter = rc;
	const char* cptr = &receivedCharacter;

	switch(receivedCharacter){
		
		//when the user hits "enter", evaluate the command, then write the prompt back out
		//case '>':
			//for all these, just increment a count so we know how long to make the string arraywrite the current string into the array of strings and 
		//case '<':
			
		//case ' ':
		//case '|':
		
		case '\n':
			printer->prettyPrint("", false);
			executor->evaluateCommand(currentCommand);
			clearCommand();
			printer->writePrompt(currentCommand, false);
			break;
		//if the user hits backspace, remove the last character from the command string and proceed as normal
		//this character is a destructive delete
		case 0x7F:
			if(currentCommand.length() > 0)
				currentCommand.erase(currentCommand.length() - 1, 1);	//erases the last character of the command
			
			printer->writePrompt(currentCommand, true); //true indicates that the user is typing, and prevents the shell from writing a newline
			break;
		//check for up arrow input
		case ' ':
			read(STDIN_FILENO, &receivedCharacter, 1);
			if(receivedCharacter == '['){
				read(STDIN_FILENO, &receivedCharacter, 1);
				if(receivedCharacter == 'A'){//arrow key detected
					string history = executor->getHistory('u');//u is for up
					printer->writePrompt(history, true);
				}
				else if(receivedCharacter == 'B'){
					string history = executor->getHistory('d');//d is for down
					printer->writePrompt(history, true);
				}
			}
		//if the received character is not a meta-character, add it to the command
		default:
			currentCommand.append(string(cptr));
			printer->writePrompt(currentCommand, true);
			break;
	}
	//	cout << "\nReceived Character: " << receivedCharacter << endl;
}