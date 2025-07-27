build:
	g++ -std=c++14 -c main.cpp WallMaker.cpp GW_Sort.cpp
	g++ -Wall -std=c++14 -o maker main.o WallMaker.o GW_Sort.o
	rm -f main.o WallMaker.o GW_Sort.o
	clear
	./maker
clean:
	rm maker