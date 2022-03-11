#include <iostream>
#include <fstream>
#include <openssl/sha.h>
#include <vector>
#include <filesystem>
#include <string>
#include <regex>
#include "FileTools.h"
#include "LogAndDisplay.h"
#include <ctime>
#include <chrono>
#include <thread>



//  LogAndDisplay Log("log.txt");

FileTools::FileTools(LogAndDisplay &Log) {
    this->buffer_size=1024 * 1024 * 128;  //default: 128 MB
    this->min_file_age = 730;	// default: 730 days
    lptr=&Log;
}



#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

void FileTools::printProgress(double &frac_of_one) {

    int val = (int) (frac_of_one * 100);
    int lpad = (int) ((frac_of_one) * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);

}


void FileTools::SetMinFileAge(const unsigned int &min_file_age) {
    this->min_file_age = min_file_age;
}

unsigned int FileTools::GetMinFileAge() {
    return this->min_file_age;
}

void FileTools::SetBufferSize(const unsigned int &buffer_size) {
    this->buffer_size = buffer_size;
}


bool FileTools::ResolveOrphanedTemp(const std::filesystem::directory_entry &entry) {

    bool error = false;

    if(entry.is_regular_file()) {

        const char *path_str = entry.path().c_str();
        std::string s(path_str);
        std::regex self_regex(".{1,}\\.temp");

        if (std::regex_search(path_str, self_regex)) {
            std::string ofn = FilenameToNormal(path_str);
            lptr->log("Orphaned temp file found: "+ s,"both");
            if(std::filesystem::exists(ofn)) {

                lptr->log("Orginal file also found: "+ofn,"both");
                lptr->log("Deleting: "+s +" ...","both");

                if(remove(path_str) !=0) {
                    lptr->log("Deleting: "+s +" failed","both");
                    error = true;
                }
            }
            RenameFileToNormal(path_str);
        }
    }
    return(error);
}



bool FileTools::RefreshFile(const std::filesystem::directory_entry &entry) {

    bool error = false;
    std::vector<unsigned char> checksum1;
    std::vector<unsigned char> checksum2;
    bool compare_result=false;


    //  if(entry.is_regular_file() && CheckIfFileIsOlderThan(entry,this->min_file_age)) {
    if(entry.is_regular_file()) {

        const char *path_str = entry.path().c_str();
        std::string s(path_str);

        lptr->log("Start processing filee: " + s,"both");
        lptr->log("Copying file "+s,"both");
        checksum1 = this->CopyFile(path_str);
        checksum2 = FileTools::ReadFile(&FileTools::FilenameToTemp(path_str)[0]);

        if(checksum1[0] && checksum2[0]) {
            compare_result = FileTools::CompareChecksums(checksum1, checksum2);
        }

        if(compare_result) {
            lptr->log("Checking checksums - ok","both");
        } else {
            lptr->log("Checking checksums - FAILED","both");
            error = true;
        }

        if(!error) {
            if(! remove(path_str) != 0) {
                lptr->log("Removing orginal file - ok","both");
            } else {
                error = true;
            }
        }

        if(!error) {
            if(! rename(&FileTools::FilenameToTemp(path_str)[0],path_str) != 0) {
                lptr->log("Renaming new file - ok","both");
                lptr->log("Processing file: " + s + " ends successfully","both");
            } else {
                lptr->log("Processing file: " + s + " FAILED","both");
                error = true;
            }
        }
    }
    return(!error);
} // for


bool FileTools::CheckIfFileIsOlderThan
(const std::filesystem::directory_entry &entry, const unsigned int days) {

    std::chrono::milliseconds delta_millis(days * 24 * 60 * 60 * 1000);
    const auto m_time = std::filesystem::last_write_time(entry);
    const auto now = std::filesystem::file_time_type::clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_time);

    if(time_span > delta_millis) {
        //std::cout << "true";
        return true;
    } else {
        return false;
    }
}


bool FileTools::RenameFileToNormal(const char *filename) {

    bool error = false;
    std::string s = filename;
    std::string r = ".temp";
    std::string::size_type pos = s.find(r);
    s.erase(pos,r.length());
    //rename(filename, s.c_str());
    if (rename(filename, s.c_str()) != 0) {
        error=true;
    }
    return error;
}


bool FileTools::CheckifFileExists(const char *filename) {
    bool result = false;
    if(std::filesystem::exists(filename)) {
        if(std::filesystem::is_regular_file(filename)) {
            result = true;
        }
    }
    return result;
}


bool FileTools::CheckifDirExists(const char* dirname) {

    bool result = false;
    if(std::filesystem::exists(dirname)) {
        if(std::filesystem::is_directory(dirname)) {
            result = true;
        }
    }
    return result;
}



bool FileTools::CompareChecksums
(std::vector<unsigned char> &checksum1, std::vector<unsigned char> &checksum2) {

    bool result = true;
    for(int i=0; i<SHA_DIGEST_LENGTH; i++) {

        if(checksum1[i] != checksum2[i]) {

            result = false;
            break;
        }
    }
    return result;
}


std::vector<unsigned char> FileTools::ReadFile(const char *filename) {

    std::string s = filename;
    std::ifstream ifs;
    ifs.open (filename, std::ifstream::binary);
    std::filebuf *ifb = ifs.rdbuf();
    std::streamsize size = ifb->pubseekoff(0,ifs.end);		// file size
    ifb->pubseekpos (0,ifs.in);
    char *buffer=new char[buffer_size];
    SHA_CTX shactx;
    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1_Init(&shactx);

    unsigned int size_left = size;

    while(ifb->sgetn(buffer,buffer_size)) {

        if(size_left > buffer_size) {
            size_left = size_left - buffer_size;
            SHA1_Update(&shactx, buffer, buffer_size);
        } else {
            SHA1_Update(&shactx, buffer, size_left);
        }
        delete[] buffer;
        buffer=new char[buffer_size];
    }

    SHA1_Final(digest, &shactx);

    ifs.close();
    buffer = nullptr;
    std::vector<unsigned char> res;
    res.assign(digest, digest+SHA_DIGEST_LENGTH);

    return res;
}



std::vector<unsigned char> FileTools::CopyFile(const char *filename) {

    std::string s = filename;
    std::ifstream ifs;
    std::ofstream ofs;

    ifs.open (filename, std::ifstream::binary);
    std::string total = std::string(filename) + std::string(".temp");

    ofs.open (total, std::ofstream::binary);

    std::filebuf *ifb = ifs.rdbuf();
    std::filebuf *ofb = ofs.rdbuf();
    std::streamsize size = ifb->pubseekoff(0,ifs.end);		// file size

    ifb->pubseekpos (0,ifs.in);
    ofb->pubseekpos (0,ofs.in);

    SHA_CTX shactx;
    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1_Init(&shactx);

    if(this->buffer_size > size) {
        this->buffer_size = size;
    }

    char *buffer=new char[buffer_size];
    unsigned int size_left = size;

    double frac_of_one = (1.0-(double(size_left))/double(size));

    while(ifb->sgetn(buffer,buffer_size)) {

        frac_of_one = (1.0-(double(size_left))/double(size));
        printProgress(frac_of_one);

        if(size_left > buffer_size) {
            size_left = size_left - buffer_size;

            ofb->sputn(buffer, buffer_size);
            SHA1_Update(&shactx, buffer, buffer_size);
        } else {

            ofb->sputn(buffer, size_left);
            SHA1_Update(&shactx, buffer, size_left);
        }

        delete[] buffer;
        buffer=new char[buffer_size];
    }
    frac_of_one = 1;
    printProgress(frac_of_one);
    std::cout << "\n\n";
    // t1.join();


    SHA1_Final(digest, &shactx);
    ifb->close();
    ofb->close();

    std::vector<unsigned char> res;
    res.assign(digest, digest+SHA_DIGEST_LENGTH);

    return res;
}


std::string FileTools::FilenameToTemp(const char *filename) {

    std::string total = std::string(filename) + std::string(".temp");
    return total;
}

std::string FileTools::FilenameToNormal(const char *filename) {

    std::string s = filename;
    std::string r = ".temp";
    std::string::size_type pos = s.find(r);
    s.erase(pos,r.length());
    return s;
}

void print_checksum(std::vector<unsigned char> checksum) {
    std::cout << "\n checksum";
    for(unsigned long i=0; i<sizeof(checksum); i++) {
        printf("%0x",checksum[i]);
    }
    std::cout << "\n";
}
