
#include "cppcheck.h"

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

    void check(const char filename[]) {
        cppcheck.check(filename);
    }

    void reportOut(const std::string &outmsg) {
        std::cout << outmsg << std::endl;
    }

    void reportErr(const ErrorLogger::ErrorMessage &msg) {
        std::cerr << msg.toString(false) << std::endl;
    }
/*
    void reportProgress(const std::string &filename,
                        const char stage[],
                        const unsigned int value) {
    }
*/
};


int main(int argc, char **argv)
{
    std::cout << "ublinter" << std::endl;

    if (argc == 2) {
        CppcheckExecutor cppcheckExecutor;
        cppcheckExecutor.check(argv[1]);
    }

    return EXIT_SUCCESS;
}

