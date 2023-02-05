CC = gcc
CFLAGS = -g -Wall -Wextra

#CC = clang-12
#CFLAGS = -Weverything

SRCS=$(wildcard src/*.c)

TARGET = main

all: $(TARGET)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	$(RM) $(TARGET)