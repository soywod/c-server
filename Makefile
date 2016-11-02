CC=gcc
CFLAGS=-W -Wall
SOURCES=$(wildcard *.c)
EXEC=server

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXEC)
