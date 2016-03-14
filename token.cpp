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
	printer = p;
}

void Executor::updateHistory(string command){
	
//	string debug = "History updated";
//	printer->prettyPrint(debug, false);
}
