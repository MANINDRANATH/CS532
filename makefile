CC=gcc
CFLAGS=-Wall -g
TARGET=sort

all: $(TARGET)

$(TARGET): sort.o
	$(CC) $(CFLAGS) -o $(TARGET) sort.o

sort.o: sort.c
	$(CC) $(CFLAGS) -c sort.c

clean:
	rm -f $(TARGET) *.o

.PHONY: clean
