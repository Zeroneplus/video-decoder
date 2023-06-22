
CC = g++

CSRC = FileStream.cpp main.cpp NalUnit.cpp
COBJ = $(CSRC:.cpp=.o)
CFLAGS = -g -O0

$(COBJ) : %.o:%.cpp
	@$(CC) $(CFLAGS) -o $@ -c $^

main : $(COBJ)
	@$(CC) $(CFLAGS) -o $@ $^

ffmpeg_test: ffmpeg_test.c
	gcc $(CFLAGS) -lavformat -lavutil -lavcodec -o $@ $^

all: main ffmpeg_test

clean:
	@-rm main $(COBJ) ffmpeg_test

format:
	clang-format -i --style=Webkit *.cpp *.h *.c

.PHONY:clean all format