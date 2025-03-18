CC = gcc
CFLAGS = -Wall -Wextra -std=c11 $(shell pkg-config --cflags libpq)
LIBS = $(shell pkg-config --libs libpq)
SRC = test.c
OUT = test

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LIBS)

clean:
	rm -f $(OUT)
