
dlint:	lintuninitvar.cpp
	g++ -g -o dlint lintuninitvar.cpp -I../cppcheck/lib ../cppcheck/cli/*.cpp ../cppcheck/lib/*.cpp

