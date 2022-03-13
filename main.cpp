#include <filesystem>
#include "LogAndDisplay.h"
#include "FileTools.h"
#include "CmdTools.h"
#include <signal.h>
#include <thread>
#include <chrono>



#define RECURSIVE "-R"
#define DRY_RUN "-d"
#define OLDER_THAN "-o"
#define BUFFER_SIZE "-b"
#define MISSING_PATH "Error: Missing path"
#define PATH_D "Error: The path doesn't exist. Path: "
#define EXITING "Exiting... \n"
#define USAGE  "\nUsage: \n\tDataRefresher /path/to/directory/ [Options]\n\nOptions:\n\t -R - recursive\n" \
"\t -o (n) - Process only files older than (n) days. Default: 730 days.\n" \
"\t -b (n) - Buffer size in Megabytes. Default: 128 MB \n" \
"Example: \n\t DataRefresher /path/to/directory/ -R -o 365 \n" 
#define LOG_FILE "DataRefresheR.log"


//static
static std::vector<std::string> available_options= {"-R","-d","-o","-b"};
bool stop = false;


// function declarations
void catch_int(int sig_num);


// objects
LogAndDisplay Log(LOG_FILE);
FileTools ft(Log);


int main(int argc, char** argv) {
	
	signal(SIGINT, catch_int);

    CmdTools ct(available_options, argc, argv);

    if(std::get<0>(ct.CheckIfOptionIsSet(OLDER_THAN))) {

		std::string s = std::get<1>(ct.CheckIfOptionIsSet(OLDER_THAN));
        ft.SetMinFileAge(stoi(s));
    }
    
        if(std::get<0>(ct.CheckIfOptionIsSet(BUFFER_SIZE))) {

		std::string s = std::get<1>(ct.CheckIfOptionIsSet(BUFFER_SIZE));
        ft.SetBufferSize(stoi(s)*1024*1024);
    }

    std::string path;
    bool error=false;

    if(argv[1]) {
        path = argv[1];

    } else {
        Log.log(MISSING_PATH,"both");
        std::cout << USAGE;
        error=true;
        return EXIT_FAILURE;
    }


    if(!error) {
        if(! ft.CheckifDirExists(&path[0])) {
            Log.log(PATH_D+path,"both");
            std::cout << USAGE;
            error = true;
            return EXIT_FAILURE;
        }
    }

	if(std::filesystem::is_empty(path)) {
		std::cout << "No files found...";
	}


    if(!std::get<0>(ct.CheckIfOptionIsSet(RECURSIVE))) {

        for (const auto &entry : std::filesystem::directory_iterator(path)) {
            ft.ResolveOrphanedTemp(entry);
            if(stop) { break;}
        }
        for (const auto &entry : std::filesystem::directory_iterator(path)) {
            ft.RefreshFile(entry);
            if(stop) { break;}
        }
    } else {

			for (const auto &entry : std::filesystem::recursive_directory_iterator(path)) {
                ft.ResolveOrphanedTemp(entry);
                if(stop) { break;}
            }

            for (const auto &entry : std::filesystem::directory_iterator(path)) {
                ft.RefreshFile(entry);
                if(stop) { break;}
            }
        }
    }
    

void catch_int(int sig_num)
{
    printf(EXITING);
    fflush(stdout);
    stop = true;
}















