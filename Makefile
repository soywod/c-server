CC=gcc
CFLAGS=-W -Wall
SOURCES=$(wildcard *.c)
EXEC=server.out

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXEC)
