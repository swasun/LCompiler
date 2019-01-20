CC=gcc

# Directories used
SRCDIR=src
HEADDIR=headers
LIBDIR=obj
BINDIR=bin

# Compilation flags
CFLAGS= -g -pedantic-errors -lpthread -Wextra -Wshadow -Werror -std=c99 -lm
GLLIBS =

# Executable name
BIN=l_compiler

# Source files
SRC= $(wildcard $(SRCDIR)/*.c)
OBJ= $(SRC:$(SRCDIR)/%.c=$(LIBDIR)/%.o)

all: $(BIN)

# Build the executable
$(BIN) : %: $(OBJ)
		$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(GLLIBS)

# Build object files from .c files
$(LIBDIR)/%.o: $(SRCDIR)/%.c $(HEADDIR)/%.h
		$(CC) -o $@ -c $< $(CFLAGS)

# Clean all objects
clean:
	rm $(LIBDIR)/*

# Clean all objects and executable
cleanall :: clean
		rm $(BINDIR)/*
