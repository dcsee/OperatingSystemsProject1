#ifndef Printer_H
#define Printer_H

//#define STDOUT_FILENO 1

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

class Printer{
	
	public:
		Printer();
		virtual ~Printer();
	
		//writes a formatted output string to the screen using write(), taking care of all
		//necessary details to make the interface user-friendly	
		void writePrompt(string text, bool isTyping);
		void prettyPrint(string text, bool isTyping);
		//returns either the full path to the working directory, or the formatted path
		string getFormattedWorkingDirectory();
	
	private:

		int lengthOfLastPrint;
		string formattedWorkingDirectory;
		string lastText;
		string getWhitespace(string thisText);
		string getBackspaces();
		
};

#endif //Printer_H