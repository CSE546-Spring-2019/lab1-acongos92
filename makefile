# Victor Congos Makefile for Lab 1 
CC = gcc
all: count

count: count.c 
	$(CC) count.c -o count

clean:
	rm count