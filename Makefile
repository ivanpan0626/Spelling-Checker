CC := gcc
CFLAGS := -g -Wall -Wno-unused-variable -fsanitize=address,undefined

all: spchk

spchk: spchk.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f spchk