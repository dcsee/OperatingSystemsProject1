#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>

#include <iostream>

#include "charParser.h"
#include "Printer.h"

using namespace std;

//used for non-canonical reads, sample code Smartsite
void SetNonCanonicalMode(int fd, struct termios *savedattributes);
//used for non-canonical reads, sample code Smartsite
void ResetCanonicalMode(int fd, struct termios *savedattributes);

int main(){

		//used for non-canonical reads, sample code Smartsite
    struct termios SavedTermAttributes;

    char incomingCharacter;		
		Printer* printer = new Printer();
		CharParser* charParser = new CharParser(printer);
    
		//set reads to non-canonical. From sample code on SmartSite
    SetNonCanonicalMode(STDIN_FILENO, &SavedTermAttributes);
		
		printer->writePrompt("", false);
		
		//infinite read loop
		while(1){

			read(STDIN_FILENO, &incomingCharacter, 1);
			if(incomingCharacter != NULL){
				charParser->receiveChar(incomingCharacter);
			}
		}
   
    ResetCanonicalMode(STDIN_FILENO, &SavedTermAttributes);
    return 0;
}

//used for non-canonical reads, sample code Smartsite
void ResetCanonicalMode(int fd, struct termios *savedattributes){
    tcsetattr(fd, TCSANOW, savedattributes);
}

//used for non-canonical reads, sample code Smartsite
void SetNonCanonicalMode(int fd, struct termios *savedattributes){
    struct termios TermAttributes;
    char *name;
    
    // Make sure stdin is a terminal. 
    if(!isatty(fd)){
        fprintf (stderr, "Not a terminal.\n");
        exit(0);
    }
    
    // Save the terminal attributes so we can restore them later. 
    tcgetattr(fd, savedattributes);
    
    // Set the funny terminal modes. 
    tcgetattr (fd, &TermAttributes);
    TermAttributes.c_lflag &= ~(ICANON | ECHO); // Clear ICANON and ECHO. 
    TermAttributes.c_cc[VMIN] = 1;
    TermAttributes.c_cc[VTIME] = 0;
    tcsetattr(fd, TCSAFLUSH, &TermAttributes);
}