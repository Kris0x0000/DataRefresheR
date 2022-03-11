#ifndef FileTools_H
#define FileTools_H
#include <vector>
#include "LogAndDisplay.h"

class FileTools {

    public:
		FileTools(LogAndDisplay &Log);
		
        bool RenameFileToNormal(const char *filename);
        bool CheckifFileExists(const char *filename);
        bool CheckifDirExists(const char* dirname);
        bool CompareChecksums(std::vector<unsigned char> &checksum1, std::vector<unsigned char> &checksum2);
        std::vector<unsigned char> ReadFile(const char *filename);
        std::vector<unsigned char> CopyFile(const char *filename);
        bool RefreshFile(const std::filesystem::directory_entry &entry);
        std::string FilenameToTemp(const char *filename);
        std::string FilenameToNormal(const char *filename);
        bool ResolveOrphanedTemp(const std::filesystem::directory_entry &entry);
        bool CheckIfFileIsOlderThan(const std::filesystem::directory_entry &entry, const unsigned int days);
        void SetMinFileAge(const unsigned int &min_file_age);
        void SetBufferSize(const unsigned int &buffer_size);
        void print_checksum(std::vector<unsigned char> checksum);
        unsigned int GetMinFileAge();
        void printProgress(double &percentage);

    private:
		long buffer_size;
		unsigned int min_file_age;
		LogAndDisplay *lptr;
		
		
};
#endif
