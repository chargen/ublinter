
all:	ublinter

clean:
	rm -rf ublinter.exe *.o

main.o:	main.cpp
	g++ -g -I. -Iexternals/cppcheck -c main.cpp

lintuninitvar.o:	lintuninitvar.cpp
	g++ -g -I. -Iexternals/cppcheck -c lintuninitvar.cpp

cppcheck.o:	externals/cppcheck/cppcheck.cpp
	g++ -g -I. -Iexternals/cppcheck -c externals/cppcheck/cppcheck.cpp

errorlogger.o:	externals/cppcheck/errorlogger.cpp
	g++ -g -I. -Iexternals/cppcheck -c externals/cppcheck/errorlogger.cpp

executionpath.o:	externals/cppcheck/executionpath.cpp
	g++ -g -I. -Iexternals/cppcheck -c externals/cppcheck/executionpath.cpp

filelister.o:	externals/cppcheck/filelister.cpp
	g++ -g -I. -Iexternals/cppcheck -c externals/cppcheck/filelister.cpp

mathlib.o:	externals/cppcheck/mathlib.cpp
	g++ -g -I. -Iexternals/cppcheck -c externals/cppcheck/mathlib.cpp

path.o:	externals/cppcheck/path.cpp
	g++ -g -I. -Iexternals/cppcheck -c externals/cppcheck/path.cpp

preprocessor.o:	externals/cppcheck/preprocessor.cpp
	g++ -g -I. -Iexternals/cppcheck -c externals/cppcheck/preprocessor.cpp

settings.o:	externals/cppcheck/settings.cpp
	g++ -g -I. -Iexternals/cppcheck -c externals/cppcheck/settings.cpp

suppressions.o:	externals/cppcheck/suppressions.cpp
	g++ -g -I. -Iexternals/cppcheck -c externals/cppcheck/suppressions.cpp

symboldatabase.o:	externals/cppcheck/symboldatabase.cpp
	g++ -g -I. -Iexternals/cppcheck -c externals/cppcheck/symboldatabase.cpp

templatesimplifier.o:	externals/cppcheck/templatesimplifier.cpp
	g++ -g -I. -Iexternals/cppcheck -c externals/cppcheck/templatesimplifier.cpp

timer.o:	externals/cppcheck/timer.cpp
	g++ -g -I. -Iexternals/cppcheck -c externals/cppcheck/timer.cpp

token.o:	externals/cppcheck/token.cpp
	g++ -g -I. -Iexternals/cppcheck -c externals/cppcheck/token.cpp

tokenize.o:	externals/cppcheck/tokenize.cpp
	g++ -g -I. -Iexternals/cppcheck -c externals/cppcheck/tokenize.cpp

tokenlist.o:	externals/cppcheck/tokenlist.cpp
	g++ -g -I. -Iexternals/cppcheck -c externals/cppcheck/tokenlist.cpp

ublinter:	main.o lintuninitvar.o cppcheck.o errorlogger.o executionpath.o filelister.o mathlib.o path.o preprocessor.o settings.o suppressions.o symboldatabase.o templatesimplifier.o timer.o token.o tokenize.o tokenlist.o
	g++ -g -o ublinter *.o -lshlwapi
