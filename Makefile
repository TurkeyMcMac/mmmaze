exe = mmmaze
CFLAGS = -std=c89 -pedantic -Wall -Wextra
LDLIBS = -lcurses

CC ?= cc
RM ?= rm -f

.PHONY: all
all: $(exe)

$(exe): src/*
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) src/*.c $(LDLIBS)

.PHONY: clean
clean:
	$(RM) $(exe)
