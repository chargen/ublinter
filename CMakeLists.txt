cmake_minimum_required(VERSION 2.6)
project(ublinter)

include_directories(. externals/cppcheck)

if (WIN32 AND NOT CYGWIN)
    set(LIBS shlwapi)
endif (WIN32 AND NOT CYGWIN)

set(CMAKE_BUILD_TYPE Debug)

add_executable(ublinter
    main.cpp
    lintuninitvar.cpp
    externals/cppcheck/cppcheck.cpp
    externals/cppcheck/errorlogger.cpp
    externals/cppcheck/executionpath.cpp
    externals/cppcheck/filelister.cpp
    externals/cppcheck/mathlib.cpp
    externals/cppcheck/path.cpp
    externals/cppcheck/preprocessor.cpp
    externals/cppcheck/settings.cpp
    externals/cppcheck/suppressions.cpp
    externals/cppcheck/symboldatabase.cpp
    externals/cppcheck/templatesimplifier.cpp
    externals/cppcheck/timer.cpp
    externals/cppcheck/token.cpp
    externals/cppcheck/tokenize.cpp
    externals/cppcheck/tokenlist.cpp
)

target_link_libraries(ublinter ${LIBS})

