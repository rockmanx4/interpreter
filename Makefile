all:
	g++ -std=c++98 -Wall -g3 interpreter.cpp -o interpreter
clean:
	rm interpreter
%: %.cpp
	g++ -std=c++98 -Wall -g3 $^ -o $@