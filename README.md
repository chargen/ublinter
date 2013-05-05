ublinter
========

Linter tool that detect undefined behaviour (only checks for using uninitialized variables yet). This is a noisy tool. The goal is to detect all bugs in standard C/C++ code.

ublinter doesn't look for syntax errors. Checking non-standard C/C++ code should be possible but there is no guarantee ublinter will detect all bugs.

Compiling:

You must have the Cppcheck source code in the folder ../cppcheck

Type: make

