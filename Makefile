
CC = gcc
CXX = g++

CXXSRC = FileStream.cpp main.cpp NalUnit.cpp \
       VideoDecoder.cpp SPS.cpp PPS.cpp
CXXOBJ = $(CXXSRC:.cpp=.o)
CFLAGS = -g -O0

$(CXXOBJ) : %.o:%.cpp
	@$(CXX) $(CFLAGS) -o $@ -c $<

main : $(CXXOBJ)
	@$(CXX) $(CFLAGS) -o $@ $^

ffmpeg_test: ffmpeg_test.c
	@$(CC) $(CFLAGS) -lavformat -lavutil -lavcodec -o $@ $^

all: main ffmpeg_test

clean:
	@-rm main $(COBJ) ffmpeg_test

format:
	clang-format -i --style=Webkit *.cpp *.h *.c

.PHONY:clean all format