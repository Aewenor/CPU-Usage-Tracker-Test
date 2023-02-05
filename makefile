#CC = gcc
#CFLAGS = -g -Wall -Wextra

CC = clang-12
CFLAGS = -Weverything

SRCS=$(wildcard src/*.c)

all: $(TARGET)
	$(CC) $(CFLAGS) -o main $(SRCS)

test: test
	$(CC) $(CFLAGS) -o test.o src/test/test.c

clean:
	$(RM) $(TARGET)