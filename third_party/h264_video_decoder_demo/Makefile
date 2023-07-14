
CXX=g++
CC=gcc

CXXSRC=$(wildcard *.cpp)

DEBUG_FLAG = -g -O0 -Werror
CFLAGS = $(DEBUG_FLAG)
CXXFLAGS = $(DEBUG_FLAG)

main:$(CXXSRC)
	@-$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	@-rm -rf main

.PHONY: clean
