# This is a simple Makefile for small projects.  When you  
# type make at the command prompt, it will compile the code.
# For more depth, see http://www.gnu.org/software/make/manual/make.html

CC=g++
CFLAGS=-lglut -lSDL2 -lGLU -lGL -lm -lstdc++ 

main: main.c
	$(CC) -o main main.c audio.c  $(CFLAGS)