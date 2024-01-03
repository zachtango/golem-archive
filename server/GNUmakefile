# This is the GNU Make shim for Linux and macOS

DESTDIR ?=
prefix ?= /usr/local

server.out: default
	./build server.out

default:
	$(MAKE) -C uSockets
	$(CC) build.c -o build
