server.out: default
	./build server.out

default: uSockets Game Room Crystals
	gcc build.c -o build

Crystals:
	g++ src/Crystals.cpp -c -march=native -O3 -Wpedantic -Wall -Wextra -Wsign-conversion -Wconversion -std=c++20 -Iinclude

Game:
	g++ src/Game.cpp -c -march=native -O3 -Wpedantic -Wall -Wextra -Wsign-conversion -Wconversion -std=c++20 -Iinclude

Room:
	g++ src/Room.cpp -c -march=native -O3 -Wpedantic -Wall -Wextra -Wsign-conversion -Wconversion -std=c++20 -Iinclude

uSockets:
	make -C uSockets

clean:
	rm *.o; rm *.out