ashell.out: main.o charParser.o Executor.o Printer.o
	g++ main.o charParser.o Executor.o Printer.o -o ashell.out

CharParser.o: charParser.h charParser.cpp
	g++ -c charParser.cpp
	
Executor.o: Executor.h Executor.cpp
	g++ -c Executor.cpp
	
Printer.o: Printer.h Printer.cpp
	g++ -c Printer.cpp
	
main.o: main.cpp
	g++ -c main.cpp
	
clean:
	rm *.o *.out