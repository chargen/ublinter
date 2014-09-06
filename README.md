ublinter
========

Linter for C/C++ code with a focus on undefined behaviour. Current checkers:
 * division by zero
 * integer overflow

The goal is to add checkers for all undefined behaviour.

ublinter doesn't look for syntax errors.


False positives / False negatives
---------------------------------

ublinter will warn when the code looks suspicious.

When using ublinter, lots of false positives must be expected.

Normally, static analysis tools are used during development. The tools are perhaps used by the developers before every commit. And the developers try to fix all the warnings the tools produce. ublinter is not intended to be used in this way - the false positives makes this usage impractical.

The recommendation is that you use ublinter as a verification tool. For instance:
 * After you or your team has worked for a while on a functionality, you can use ublinter as a verification that the functionality works. Run ublinter on your file and look at all warnings.
 * When a release candidate is created you could run ublinter on it and look at all warnings.
 * Run ublinter once a week on your source code, diff the results and only investigate new results.
 * etc

You can use ublinter as a complement to your static analysis tools.

Design ideas
------------

ublinter will assume that all conditions can always be both true/false. There will be no, or very little, handling of impossible execution paths.

ublinter will assume that signed variables can be negative. perhaps in same cases it should even be assumed that variables have *any* value.

Usage
-----

Cppcheck is used to parse the code and create a dumpfile. Then the dump file is read by ublinter and it looks for UB.

To scan a file 1.c with ublinter:

    cppcheck --dump 1.c
    python ublinter.py 1.c.dump

The intention is that the output-format will be similar to Cppcheck. So plugins / report generators / etc can be reused for both Cppcheck and ublinter with minimal effort.

