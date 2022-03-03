#include <iostream>
#include <fstream>
#include "LogAndDisplay.h"
#include <ctime>
#include <time.h>
#include <stdio.h>

std::ofstream ofs;
std::filebuf *ofb;


LogAndDisplay::LogAndDisplay(const std::string &filename) {

    ofs.open (filename, std::ofstream::out);
    ofb = ofs.rdbuf();
}


bool LogAndDisplay::log(const std::string &text, const std::string &mode) {

    std::string line = PrepareLine(text);
    
    if(mode == "log") {

        ofb->sputn(&line[0], line.size());

    } else if(mode == "display") {
		
        std::cout << line;

    } else if(mode == "both") {

        std::cout << line;
        ofb->sputn(&line[0], line.size());
    }

    return true;
}


std::string LogAndDisplay::PrepareLine(std::string text) {

    char buffer [20];

    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (buffer,20,"%d-%m-%Y %H:%M:%S",timeinfo);
    std::string s(buffer);
    text = s + " - " + text + "\n";

    return text;
}








