
ublinter:	lintuninitvar.cpp
	g++ -g -o ublinter lintuninitvar.cpp -I../cppcheck/lib ../cppcheck/cli/*.cpp ../cppcheck/lib/*.cpp

