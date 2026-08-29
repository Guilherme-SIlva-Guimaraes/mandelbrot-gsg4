CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -fopenmp -pthread -lm

TARGET = mandelbrot
SRC = mandelbrot.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC) mandelbrot.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.pgm times.txt
