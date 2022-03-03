#ifndef CmdTools_H
#define CmdTools_H

#include <vector>
#include <iostream>

class CmdTools {

    public:
        CmdTools(std::vector<std::string> &available_options, int argc, char **argv);
        std::tuple<bool, std::string> CheckIfOptionIsSet(std::string option);


    private:
        std::vector<std::string> CstringToString(char **array_of_arrays, int count_of_arrays);
        std::vector<std::tuple<std::string, std::string>> GetOptions();


        int argc;
        char **argv;
        std::vector<std::string> available_options;

};
#endif
