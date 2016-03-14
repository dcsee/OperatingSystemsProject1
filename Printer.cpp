#include <unistd.h>
#include <cstring>

#include "Printer.h"

#define NO_ERROR 0
#define MAX_PATH_LENGTH 16
#define PATH_DELIMITERS "/"

using namespace std;

Printer::~Printer(){
	
}

Printer::Printer(){
	lengthOfLastPrint = 0;
	lastText = "";
	formattedWorkingDirectory = getFormattedWorkingDirectory();	
}



//returns either the full path to the working directory, or the formatted path
string Printer::getFormattedWorkingDirectory(){
	
	char* workingDirectoryPath = get_current_dir_name();
	int pathLength = strlen(workingDirectoryPath);
	char* subdirectory;
	char* subdir_temp;
	
	//if the string is less than or equal to 16 characters long
	if(pathLength <= MAX_PATH_LENGTH){
		const char* wdpath = workingDirectoryPath;
		return string(wdpath);
	}

	//if the code gets here, then the path was too long, so creating a formatted path is necessary
	//finds the current WORKING directory and returns it into subdirectory, excluding the preceding directories
	//if the working directory is longer than 16 characters long, whatever
	subdir_temp = strtok(workingDirectoryPath, PATH_DELIMITERS);
	while(subdir_temp != NULL){
		subdirectory = subdir_temp;
		subdir_temp = strtok(NULL, PATH_DELIMITERS);
	}
	
	//this is necessary to create a string
	const char* subdirectoryFinal = subdirectory;
	return "/.../" + string(subdirectoryFinal);
}

string Printer::getWhitespace(string thisText){

	int lengthDifference = lastText.length() - thisText.length();
	char whitespace = 0x20;
	char backspace = 0x8;
	
	//if the last string was longer than the current string, return enough whitespace to overwrite the leftovers
	if(lengthDifference > 0){
		return string(lengthDifference, whitespace) + string(lengthDifference, backspace);
	}
	else
		return "";
}

string Printer::getBackspaces(){
	char backspace = 0x8;
	return string(lastText.length(), backspace);
}

//prints a string of text in human-readable form, on its own line
void Printer::prettyPrint(string textToPrint, bool isTyping){

	//first determine if you need to print a newline
	char nL = 0xA;
	string newLine = "";
	
	//if text was previously printed, and the user is not typing, then put the new text on its own line
	if((lastText.length() > 0) && isTyping != true){
		newLine = string(1, nL);
	}
	
	//this is the exact string of characters to put on the screen
	string line = getBackspaces() + newLine + textToPrint + getWhitespace(textToPrint);
	const char* lineBuffer = line.c_str();
	write(STDOUT_FILENO, lineBuffer, line.length());
	
	//The last thing that was printed. Used to determine where to position the cursor during the next print.
	//Does NOT including backspaces, whitespace, or newlines
	lastText = textToPrint;
}

void Printer::writePrompt(string textToPrint, bool isTyping){

	//the prompt is the working directory, plus all the text in front of the carrot
	string prompt = formattedWorkingDirectory + ">" + textToPrint;
	prettyPrint(prompt, isTyping);
}