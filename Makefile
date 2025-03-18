CC = gcc
CFLAGS = -Wall -Wextra -std=c11 $(shell pkg-config --cflags libpq)
LIBS = $(shell pkg-config --libs libpq)
SRC = test.c fonctions.o
OUT = test

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LIBS)

fonctions.o : fonctions.c fonctions.h
	$(CC) -c fonctions.c

clean:
	rm -f $(OUT) fonctions.o
