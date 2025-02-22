# Make file
CC = gcc
CFLAGS = -Wall -Wextra -O2
SRC = src/main.c
BIN = bin/twrter

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)

clean:
	rm -f bin/*
