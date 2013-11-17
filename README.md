ublinter
========

Linter tool for C/C++ code. Only checks for using uninitialized variables yet,
but the goal is to detect all UB. This is a noisy tool. The goal is to detect
all UB bugs in standard C99 code.

ublinter doesn't look for syntax errors. Checking non-standard C/C++ code should
be possible but there is no guarantee ublinter will detect all bugs.

Design ideas:
 * ublinter will assume that all conditions can always be both true/false.
   There will be no, or very little, handling of impossible execution paths.
 * When calling functions, ublinter will make the assumption that any data
   passed to the function must always be initialized.

ublinter currently uses the cppcheck-core to parse code. I've looked at other
parsers too (gccxml, clang, frama-c) but this was the parser I could get started with
quickest. I might switch from cppcheck to another parser later.

Limitations
-----------

The intention is that the uninitialized variables checking will only look for cases where there is UB.

I don't intend to report errors about global/static variables:

    #include <stdio.h>

    int x;

    int main() {
        printf("%i\n", x); // No UB => no error message
        return 0;
    }

I don't intend to report errors about standard classes:

    #include <string>
    #include <iostream>

    int main() {
        std::string str;
        std::cout << str << std::endl; // No UB => no error
        return 0;
    }


Compiling
---------

You should be able to use almost any C++ compiler to compile ublinter.

Linux:

    make

Windows:

    make LDFLAGS=-lshlwapi

Flags:

    CXX      : Compiler
    CXXFLAGS : Compiler flags
    CPPFLAGS : Preprocessor flags (include folders etc)
    LDFLAGS  : Linker flags
