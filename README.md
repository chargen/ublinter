ublinter
========

Linter tool for C/C++ code. Only checks for:
 * division by zero
but the goal is to detect all UB. This is a noisy but sound tool. The goal is to detect
all UB bugs in standard C99 code.

ublinter doesn't look for syntax errors. Checking non-standard C/C++ code should
be possible but there is no guarantee ublinter will detect all bugs.

Design ideas:
 * ublinter will assume that all conditions can always be both true/false.
   There will be no, or very little, handling of impossible execution paths.

Usage
-----

This tool is a Cppcheck extension. It relies on Cppcheck dumpfiles.

Right now there is no proper tool support for Cppcheck extensions, to use this script you must manually create the dump file and call this script.

To scan a file 1.c:
# cppcheck --dump-file 1.c
# python ublinter.py 1.c.dump

This usage is far from ideal. So this is hopefully only temporary and will be improved later.
