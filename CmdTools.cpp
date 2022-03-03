#include <array>
#include <tuple>
#include "CmdTools.h"


CmdTools::CmdTools(std::vector<std::string> &available_options, int argc, char **argv) {

    this->argc=argc;
    this->argv=argv;
    this->available_options=available_options;
}

std::vector<std::string> CmdTools::CstringToString(char **array_of_arrays, int count_of_arrays) {

    std::vector<std::string> res;

    for(long i=0; i<count_of_arrays; i++) {

        std::string s(array_of_arrays[i]);
        res.push_back(s);
    }
    return res;
}


std::vector<std::tuple<std::string, std::string>> CmdTools::GetOptions() {

    std::vector<std::tuple<std::string, std::string>> activated_options;

    std::vector<std::string> options = CstringToString(this->argv, argc);
    std::tuple<std::string, std::string> row;

    for(unsigned int i=2; i< options.size(); i++) {
        for(unsigned int j=0; j< available_options.size(); j++) {
            if(options.at(i) == available_options.at(j)) {

                if(i+1 < options.size()) {
                    row = std::make_tuple (options.at(i),options.at(i+1) );

                } else {
                    row = std::make_tuple (options.at(i),"" );
                }
            }
            activated_options.push_back(row);
        }
    }
    return activated_options;
}



std::tuple<bool, std::string> CmdTools::CheckIfOptionIsSet(std::string option) {

    std::vector<std::tuple<std::string, std::string>> options_selected = GetOptions();
    std::tuple<std::string, std::string> tup;
    std::tuple<bool, std::string> res = std::make_tuple(false, "");

    for(unsigned i=0; i< options_selected.size(); ++i) {
        tup = options_selected.at(i);

        std::string opt = std::get<0>(tup);
        std::string argument = std::get<1>(tup);

        if(option == opt) {
            res = std::make_tuple(true, argument);
            break;
        }
    }
    return res;
}
