#ifndef checkunusedfunctionsH
#define checkunusedfunctionsH

#include "tokenize.h"
#include "errorlogger.h"

class CheckUnusedFunctions {
public:
    void parseTokens(Tokenizer &, const char*, Settings*) {}
    void check(ErrorLogger * const) {}
};

#endif
