# ==========================
#   Makefile for myshell
# ==========================

# Compiler and flags
CC      = gcc
CFLAGS  = -Wall -Werror -Iinclude

# Directories
SRCDIR  = src
INCDIR  = include
OBJDIR  = obj
BINDIR  = bin

# Files
OBJS    = $(OBJDIR)/main.o $(OBJDIR)/shell.o $(OBJDIR)/execute.o
TARGET  = $(BINDIR)/myshell

# Default target
all: $(TARGET)

# Link all objects into final executable
$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) -o $@ $^

# Compile .c files into .o files in obj/
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/shell.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "Cleaned all build files."

# Run target (optional)
run: $(TARGET)
	@echo "Running myshell..."
	@./$(TARGET)

.PHONY: all clean run
