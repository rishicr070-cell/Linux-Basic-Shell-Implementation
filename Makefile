# Makefile for Linux Basic Shell
# Component 12: Deployment Automation
# Author: Rishi
# Date: January 4, 2026

CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99
TARGET = shell
SRC = shell.c

# Color codes for output
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
RED = \033[1;31m
RESET = \033[0m

.PHONY: all clean run test help install

# Default target
all: $(TARGET)
	@echo "$(GREEN)✓ Build completed successfully!$(RESET)"
	@echo "$(BLUE)Run './$(TARGET)' or 'make run' to start the shell$(RESET)"

# Compile the shell
$(TARGET): $(SRC)
	@echo "$(YELLOW)Compiling $(SRC)...$(RESET)"
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)
	@chmod +x $(TARGET)

# Run the shell
run: $(TARGET)
	@echo "$(GREEN)Starting Linux Basic Shell...$(RESET)"
	@./$(TARGET)

# Clean build artifacts
clean:
	@echo "$(YELLOW)Cleaning build artifacts...$(RESET)"
	@rm -f $(TARGET) *.o *~ core
	@echo "$(GREEN)✓ Clean completed!$(RESET)"

# Run tests
test: $(TARGET)
	@echo "$(BLUE)Running automated tests...$(RESET)"
	@bash tests/demo.sh

# Install to /usr/local/bin (requires sudo)
install: $(TARGET)
	@echo "$(YELLOW)Installing to /usr/local/bin...$(RESET)"
	@sudo cp $(TARGET) /usr/local/bin/myshell
	@echo "$(GREEN)✓ Installed! Run 'myshell' from anywhere$(RESET)"

# Uninstall from system
uninstall:
	@echo "$(YELLOW)Uninstalling from /usr/local/bin...$(RESET)"
	@sudo rm -f /usr/local/bin/myshell
	@echo "$(GREEN)✓ Uninstalled!$(RESET)"

# Show help
help:
	@echo "$(BLUE)════════════════════════════════════════════════$(RESET)"
	@echo "$(GREEN)Linux Basic Shell - Makefile Help$(RESET)"
	@echo "$(BLUE)════════════════════════════════════════════════$(RESET)"
	@echo ""
	@echo "$(YELLOW)Available targets:$(RESET)"
	@echo "  make           - Compile the shell"
	@echo "  make run       - Compile and run the shell"
	@echo "  make test      - Run automated tests"
	@echo "  make clean     - Remove compiled files"
	@echo "  make install   - Install to system (requires sudo)"
	@echo "  make uninstall - Remove from system"
	@echo "  make help      - Show this help message"
	@echo ""
	@echo "$(YELLOW)Examples:$(RESET)"
	@echo "  make && ./shell"
	@echo "  make run"
	@echo "  make test"
	@echo ""

# Debug build with extra symbols
debug: CFLAGS += -DDEBUG -O0
debug: clean $(TARGET)
	@echo "$(GREEN)✓ Debug build completed!$(RESET)"

# Release build with optimizations
release: CFLAGS += -O3
release: clean $(TARGET)
	@echo "$(GREEN)✓ Release build completed!$(RESET)"

# Check for memory leaks using valgrind
valgrind: $(TARGET)
	@echo "$(YELLOW)Running valgrind memory check...$(RESET)"
	@echo "Type 'exit' to quit the shell after testing"
	@valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Archive project for submission
archive:
	@echo "$(YELLOW)Creating submission archive...$(RESET)"
	@tar -czf simple-shell-submission.tar.gz \
		shell.c Makefile README.md tests/ docs/ screenshots/ 2>/dev/null || true
	@echo "$(GREEN)✓ Archive created: simple-shell-submission.tar.gz$(RESET)"
