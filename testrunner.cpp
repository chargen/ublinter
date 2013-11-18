#include "cppcheck.h"
#include "filelister.h"
#include "path.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>

class CppcheckExecutor : public ErrorLogger {
private:
    CppCheck cppcheck;
    std::set<int> bad;
    int fail;

public:
    CppcheckExecutor()
        : ErrorLogger()
        , cppcheck(*this,false)
        , fail(0) {
        cppcheck.dontSimplify();
    }

    int test(const std::string &filename) {
        bad.clear();
        fail = 0;
        std::set<int> lines;
        std::ifstream fin(filename.c_str());
        if (fin.is_open()) {
            std::string str;
            int linenr = 1;
            while (std::getline(fin,str)) {
                if (str.find("BAD") != std::string::npos || str.find("FP") != std::string::npos)
                    lines.insert(linenr);
                linenr++;
            }
            fin.close();
        }

        cppcheck.check(filename);

        for (std::set<int>::const_iterator it = lines.begin(); it != lines.end(); ++it) {
            if (bad.find(*it) == bad.end()) {
                fail++;
                std::cerr << "Not detected: filename=" << filename << " line=" << *it << std::endl;
            }
        }

        for (std::set<int>::const_iterator it = bad.begin(); it != bad.end(); ++it) {
            if (lines.find(*it) == lines.end()) {
                fail++;
                std::cerr << "False positive: filename=" << filename << " line=" << *it << std::endl;
            }
        }

        return fail;
    }

    void reportOut(const std::string &) {}

    void reportErr(const ErrorLogger::ErrorMessage &msg) {
        bad.insert(msg._callStack.front().line);
    }
};


int main()
{
    std::map<std::string, std::size_t> files;
    FileLister::recursiveAddFiles(files, "test");

    int err = 0;

    CppcheckExecutor cppcheckExecutor;
    for (std::map<std::string,std::size_t>::const_iterator it = files.begin(); it != files.end(); ++it) {
        err += cppcheckExecutor.test(it->first);
    }

    if (err == 0)
        std::cout << "All tests PASSED" << std::endl;

    return (err == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

