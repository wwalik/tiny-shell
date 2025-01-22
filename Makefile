CC := gcc
CFLAGS := --std=c99
DBGFLAGS := -ggdb

.PHONY: clean

tsh: main.c
	$(CC) $(CFLAGS) -o $@ $<

debug: main.c
	$(CC) $(CFLAGS) $(DBGFLAGS) -o tsh-$@ $<

clean:
	rm tsh tsh-debug
