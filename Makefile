all.o: prog

prog: main.o LogAndDisplay.o FileTools.o CmdTools.o
	g++ -o all.o main.o LogAndDisplay.o FileTools.o CmdTools.o -lpthread -lssl -lcrypto -pthread -std=c++2a -Wall 

main.o: main.cpp
	g++ -c main.cpp -std=c++2a -Wall 
	
FileTools.o: FileTools.cpp FileTools.h
	g++ -c FileTools.cpp -std=c++2a -pthread -Wall 

LogAndDisplay.o: LogAndDisplay.cpp LogAndDisplay.h
	g++ -c LogAndDisplay.cpp -std=c++2a -Wall 
	
CmdTools.o: CmdTools.cpp CmdTools.h 
	g++ -c CmdTools.cpp -std=c++2a -Wall 
	


