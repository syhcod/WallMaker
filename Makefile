build:
	g++ -std=c++14 -c main.cpp WallMaker.cpp
	g++ -Wall -std=c++14 -o maker main.o WallMaker.o
	rm -f main.o WallMaker.o
	./maker
clean:
	rm maker