
#include "cppcheck.h"
#include "filelister.h"
#include "path.h"

#include <cstdlib>
#include <iostream>

class CppcheckExecutor : public ErrorLogger {
private:
    CppCheck cppcheck;

public:
    CppcheckExecutor()
        : ErrorLogger()
        , cppcheck(*this,false) {
    }

    void check(const std::string &filename) {
        cppcheck.check(filename);
    }

    void reportOut(const std::string &outmsg) {
        std::cout << outmsg << std::endl;
    }

    void reportErr(const ErrorLogger::ErrorMessage &msg) {
        std::cerr << msg.toString(false) << std::endl;
    }
};


int main(int argc, char **argv)
{
    std::cout << "ublinter" << std::endl;

    std::map<std::string, std::size_t> files;
    for (int i = 1; i < argc; i++) {
        std::string path = Path::fromNativeSeparators(argv[i]);
        path = Path::removeQuotationMarks(path);
        FileLister::recursiveAddFiles(files, Path::toNativeSeparators(path));
    }

    CppcheckExecutor cppcheckExecutor;
    for (std::map<std::string,std::size_t>::const_iterator it = files.begin(); it != files.end(); ++it) {
        cppcheckExecutor.check(it->first);
    }

    return EXIT_SUCCESS;
}

