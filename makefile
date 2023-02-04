CC = gcc
CFLAGS = -g -Wall -Wextra

#CC = clang-12
#CFLAGS = -Weverything

TARGET = main

all: $(TARGET)
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)