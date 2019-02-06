CC=gcc-8
CXX=g++-8

all: debug

.PHONY: debug
debug: build
	cd build && ninja

build:
	mkdir -p build
	cd build && CC=$(CC) CXX=$(CXX) cmake -GNinja -DCMAKE_BUILD_TYPE=Debug ..

.PHONY: clean
clean:
	cd build && ninja clean

.PHONY: fullclean
fullclean:
	rm -rf build/
