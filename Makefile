# This is a simple Makefile for small projects.  When you  
# type make at the command prompt, it will compile the code.
# For more depth, see http://www.gnu.org/software/make/manual/make.html

CC=gcc
CFLAGS=-lglut -lGLU -lGL -lm -lstdc++

main: main.cpp
	$(CC) -o main main.cpp  $(CFLAGS)



