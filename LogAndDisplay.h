#ifndef LogAndDisplay_H
#define LogAndDisplay_H
#include <iostream>
#include <fstream>


class LogAndDisplay {

        public:
        LogAndDisplay(const std::string &filename);
        bool log(const std::string &text, const std::string &mode); // mode = log | display | both 
        
    private:
        std::ofstream ofs;
        std::filebuf *ofb;
		std::string line;
		std::string PrepareLine(std::string text);
        
};
#endif
