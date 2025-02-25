# Make file
CC = gcc
CFLAGS = -Wall -O2 -Wextra -Werror -pedantic -std=c17 -D_POSIX_C_SOURCE=200809L
SRC = src/main.c
BIN = bin/twrter

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)

clean:
	rm -f bin/*
