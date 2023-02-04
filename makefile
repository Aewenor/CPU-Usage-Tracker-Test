CC = gcc
CFLAGS = -g -Wall

TARGET = main

all: $(TARGET)
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)