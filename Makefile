
CC = gcc
CXX = g++

CXXSRC = FileStream.cpp main.cpp NalUnit.cpp \
         VideoDecoder.cpp SPS.cpp PPS.cpp Slice.cpp \
		 Vui.cpp
CXXOBJ = $(CXXSRC:%.cpp=%.o)
DEBUG_FLAG = -g -O0 -Werror
CFLAGS = $(DEBUG_FLAG)
CXXFLAGS = $(DEBUG_FLAG) -std=gnu++17 -Ithird_party/spdlog/include

$(CXXOBJ) : %.o:%.cpp
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

main : $(CXXOBJ)
	@$(CXX) $(CXXFLAGS) -o $@ $^

ffmpeg_test: ffmpeg_test.c
	@$(CC) $(CFLAGS) -lavformat -lavutil -lavcodec -o $@ $^

all: main

clean:
	@-rm -f main $(COBJ) $(CXXOBJ)
	@-rm -f ffmpeg_test

format:
	@clang-format -i --style=Webkit *.cpp *.h *.c

.PHONY:all clean format