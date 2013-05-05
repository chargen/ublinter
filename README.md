dlint
=====

lint tool for C/C++ code

goal:
detect all uninitialized variables.

This tool is meant to be a noisy tool. Most/all warnings will be noise.

Unfortunately, dlint doesn't (yet) detect all uninitialized variables. Bugs inside unreachable scope are not found:

    int x;
    if (0) {
        return x; // <- bug not found, because the code is unreachable
    }

