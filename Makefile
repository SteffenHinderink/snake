run: build/Snake
	./build/Snake

build/Snake: build src/Snake.cc
	g++ -std=c++0x -pthread -o build/Snake src/Snake.cc

build:
	mkdir build

clear:
	rm -rf build
