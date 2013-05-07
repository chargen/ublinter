ublinter
========

Linter tool for C/C++ code. Only checks for using uninitialized variables yet, but the goal is to detect all UB. This is a noisy tool. The goal is to detect all UB bugs in standard C99 code.

ublinter doesn't look for syntax errors. Checking non-standard C/C++ code should be possible but there is no guarantee ublinter will detect all bugs.

design ideas:
 * ublinter will assume that all conditions can always be both true/false. there will be no, or very little, handling of impossible execution paths.
 * when calling functions, ublinter will make the assumption that any data passed to the function must always be initialized.

ublinter currently uses the cppcheck-core to parse code. I've looked at other parsers too (gccxml,clang,frama-c) but this was the parser I could get started with quickest. I might switch from cppcheck to another parser later.

Compiling:
	make

