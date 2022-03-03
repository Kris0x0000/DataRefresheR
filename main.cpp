#include <filesystem>
#include "LogAndDisplay.h"
#include "FileTools.h"
#include "CmdTools.h"


#define RECURSIVE "-R"
#define DRY_RUN "-d"
#define OLDER_THAN "-o"

//static
static std::vector<std::string> available_options= {"-R","-d","-o"};


// objects
LogAndDisplay Log("log.txt");
FileTools ft(Log);



int main(int argc, char** argv) {

    CmdTools ct(available_options, argc, argv);

    if(std::get<0>(ct.CheckIfOptionIsSet(OLDER_THAN))) {

		std::string s = std::get<1>(ct.CheckIfOptionIsSet(OLDER_THAN));
        ft.SetMinFileAge(stoi(s));
        //std::cout << std::get<1>(ct.CheckIfOptionIsSet(OLDER_THAN)) << "\n";
    }


    std::string path;
    static const std::string usage_helper =
        "\n Usage: \n DataRefresher /path/to/directory/ [Options]\n\n Options:\n -R - recursive\n"
        " -d - dry run";
    bool error=false;

    if(argv[1]) {
        path = argv[1];

    } else {
        Log.log("Missing path","both");
        std::cout << usage_helper;
        error=true;
        return EXIT_FAILURE;
    }


    if(!error) {
        if(! ft.CheckifDirExists(&path[0])) {
            Log.log("The path doesn't exist: "+path,"both");
            std::cout << usage_helper;
            error = true;
            return EXIT_FAILURE;
        }
    }



    if(!std::get<0>(ct.CheckIfOptionIsSet(RECURSIVE))) {

        for (const auto &entry : std::filesystem::directory_iterator(path)) {
            ft.ResolveOrphanedTemp(entry);
        }
        for (const auto &entry : std::filesystem::directory_iterator(path)) {
            ft.RefreshFile(entry);
        }
    } else {

        for (const auto &entry : std::filesystem::recursive_directory_iterator(path)) {

                ft.ResolveOrphanedTemp(entry);
            }

            for (const auto &entry : std::filesystem::directory_iterator(path)) {
                ft.RefreshFile(entry);
            }
        }
    }
    
















