
ifndef CXXFLAGS
    #CXXFLAGS=-pedantic -Wall -Wextra -Wabi -Wcast-qual -Wconversion -Wfloat-equal -Winline -Wmissing-declarations -Wmissing-format-attribute -Wno-long-long -Woverloaded-virtual -Wpacked -Wredundant-decls -Wshadow -Wsign-promo -g
    CXXFLAGS=-g
endif

ifndef CXX
    CXX=g++
endif

ifndef CPPFLAGS
    CPPFLAGS=-I. -Iexternals/cppcheck
endif

EXTOBJ = externals/cppcheck/errorlogger.o \
         externals/cppcheck/filelister.o \
         externals/cppcheck/tokenize.o \
         externals/cppcheck/mathlib.o \
         externals/cppcheck/timer.o \
         externals/cppcheck/settings.o \
         externals/cppcheck/symboldatabase.o \
         externals/cppcheck/preprocessor.o \
         externals/cppcheck/cppcheck.o \
         externals/cppcheck/tokenlist.o \
         externals/cppcheck/templatesimplifier.o \
         externals/cppcheck/suppressions.o \
         externals/cppcheck/executionpath.o \
         externals/cppcheck/path.o \
         externals/cppcheck/token.o

ublinter: main.o lintdivision.o lintsignedvar.o lintuninitvar.o $(EXTOBJ)
	$(CXX) $(CXXFLAGS) -o ublinter main.o lintdivision.o lintsignedvar.o lintuninitvar.o $(EXTOBJ) $(LDFLAGS)

all: ublinter testrunner

testrunner: testrunner.o lintdivision.o lintsignedvar.o lintuninitvar.o $(EXTOBJ)
	$(CXX) $(CXXFLAGS) -o testrunner  testrunner.o lintdivision.o lintsignedvar.o lintuninitvar.o $(EXTOBJ) $(LDFLAGS)

test: all
	./testrunner

clean:
	rm -f *.o externals/cppcheck/*.o testrunner ublinter

lintdivision.o: lintdivision.cpp *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c lintdivision.cpp

lintsignedvar.o: lintsignedvar.cpp *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c lintsignedvar.cpp

lintuninitvar.o: lintuninitvar.cpp *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c lintuninitvar.cpp

main.o: main.cpp *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c main.cpp

testrunner.o: testrunner.cpp *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c testrunner.cpp

externals/cppcheck/errorlogger.o: externals/cppcheck/errorlogger.cpp externals/cppcheck/*.h *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o externals/cppcheck/errorlogger.o -c externals/cppcheck/errorlogger.cpp

externals/cppcheck/filelister.o: externals/cppcheck/filelister.cpp externals/cppcheck/*.h *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o externals/cppcheck/filelister.o -c externals/cppcheck/filelister.cpp

externals/cppcheck/tokenize.o: externals/cppcheck/tokenize.cpp externals/cppcheck/*.h *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o externals/cppcheck/tokenize.o -c externals/cppcheck/tokenize.cpp

externals/cppcheck/mathlib.o: externals/cppcheck/mathlib.cpp externals/cppcheck/*.h *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o externals/cppcheck/mathlib.o -c externals/cppcheck/mathlib.cpp

externals/cppcheck/timer.o: externals/cppcheck/timer.cpp externals/cppcheck/*.h *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o externals/cppcheck/timer.o -c externals/cppcheck/timer.cpp

externals/cppcheck/settings.o: externals/cppcheck/settings.cpp externals/cppcheck/*.h *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o externals/cppcheck/settings.o -c externals/cppcheck/settings.cpp

externals/cppcheck/symboldatabase.o: externals/cppcheck/symboldatabase.cpp externals/cppcheck/*.h *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o externals/cppcheck/symboldatabase.o -c externals/cppcheck/symboldatabase.cpp

externals/cppcheck/preprocessor.o: externals/cppcheck/preprocessor.cpp externals/cppcheck/*.h *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o externals/cppcheck/preprocessor.o -c externals/cppcheck/preprocessor.cpp

externals/cppcheck/cppcheck.o: externals/cppcheck/cppcheck.cpp externals/cppcheck/*.h *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o externals/cppcheck/cppcheck.o -c externals/cppcheck/cppcheck.cpp

externals/cppcheck/tokenlist.o: externals/cppcheck/tokenlist.cpp externals/cppcheck/*.h *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o externals/cppcheck/tokenlist.o -c externals/cppcheck/tokenlist.cpp

externals/cppcheck/templatesimplifier.o: externals/cppcheck/templatesimplifier.cpp externals/cppcheck/*.h *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o externals/cppcheck/templatesimplifier.o -c externals/cppcheck/templatesimplifier.cpp

externals/cppcheck/suppressions.o: externals/cppcheck/suppressions.cpp externals/cppcheck/*.h *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o externals/cppcheck/suppressions.o -c externals/cppcheck/suppressions.cpp

externals/cppcheck/executionpath.o: externals/cppcheck/executionpath.cpp externals/cppcheck/*.h *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o externals/cppcheck/executionpath.o -c externals/cppcheck/executionpath.cpp

externals/cppcheck/path.o: externals/cppcheck/path.cpp externals/cppcheck/*.h *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o externals/cppcheck/path.o -c externals/cppcheck/path.cpp

externals/cppcheck/token.o: externals/cppcheck/token.cpp externals/cppcheck/*.h *.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o externals/cppcheck/token.o -c externals/cppcheck/token.cpp

