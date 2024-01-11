server.out: default
	./build server.out

default: usockets game room crystals
	gcc build.c -o build

crystals:
	g++ src/Crystals.cpp -c -march=native -O3 -Wpedantic -Wall -Wextra -Wsign-conversion -Wconversion -std=c++20 -Iinclude

game:
	g++ src/Game.cpp -c -march=native -O3 -Wpedantic -Wall -Wextra -Wsign-conversion -Wconversion -std=c++20 -Iinclude

room:
	g++ src/Room.cpp -c -march=native -O3 -Wpedantic -Wall -Wextra -Wsign-conversion -Wconversion -std=c++20 -Iinclude

usockets:
	make -C uSockets

clean:
	rm *.o; rm *.out; rm uSockets/*.o