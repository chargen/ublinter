ublinter
========

Linter for C/C++ code with a focus on undefined behaviour. Current checkers:
 * division by zero
 * integer overflow

The goal is to add checkers for all undefined behaviour.

ublinter doesn't look for syntax errors.


False positives / False negatives
---------------------------------

The idea is that ublinter will warn when the code looks suspicious / dangerous. it won't try to proove that there is something wrong.

Still, don't expect that ublinter is sound (detects all UB). A sound tool would warn for every operation unless it can proove that nothing bad can ever happen.

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

ublinter will assume that signed variables can be negative.

Usage
-----

Cppcheck is used to parse the code and create a dumpfile. Then the dump file is used by ublinter to look for UB.

Right now, you must manually create the cppcheck dump file and pass it to ublinter.

To scan a file 1.c with ublinter:

    cppcheck --dump 1.c
    python ublinter.py 1.c.dump

This usage is not ideal. Hopefully it will be easier in the future. At least in theory, the Cppcheck binaries (CLI+GUI+etc) could be enhanced so cppcheck extensions can be used in analysis.

The intention is that the output-format from ublinter will be similar to Cppcheck. So plugins / report generators / etc can be reused for both Cppcheck and ublinter with minimal effort.

