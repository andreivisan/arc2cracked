# Makefile for kouka build tool

TARGET = bin/kouka
# Compiler and Flags
CC     = gcc 
CFLAGS = -Wall -Wextra -02 -Include
# Source and Object files
SRC    = $(wildcard src/*.c)
OBJ    = $(patsubst src/%.c, obj/%.o, $(SRC))

