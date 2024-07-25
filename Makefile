# Variables
CC = clang
CFLAGS = -Wall -Wextra -Werror -g -Iinclude
LEXFLAGS = -Wno-error
LDFLAGS = -lhpdf
YFLAGS = -d
LEX = flex
YACC = bison

LIB_PATH = /usr/local/lib

# currently only UNIX-like agnostic
UNAME_S := $(shell uname -s)
# Check if the operating system is macOS
ifeq ($(UNAME_S),Darwin)
    LDFLAGS += -Wl,-rpath,$(LIB_PATH)
endif

# Directories
BUILD_DIR = build
PARSER_DIR = parsers
SRC_DIR = .

# Files
TARGET = $(BUILD_DIR)/throff
LEX_SRC = $(PARSER_DIR)/mom.l
YACC_SRC = $(PARSER_DIR)/mom.y
LEX_OUTPUT = $(BUILD_DIR)/lex.yy.c
YACC_OUTPUT = $(BUILD_DIR)/mom.tab.c
YACC_HEADER = $(BUILD_DIR)/mom.tab.h
OBJ_FILES = $(BUILD_DIR)/lex.yy.o $(BUILD_DIR)/mom.tab.o $(BUILD_DIR)/throff.o $(BUILD_DIR)/pdf.o

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Rule for generating the final executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ_FILES) $(LDFLAGS)

# Rule for generating the yacc output
$(YACC_OUTPUT) $(YACC_HEADER): $(YACC_SRC)
	$(YACC) $(YFLAGS) -o $(YACC_OUTPUT) $(YACC_SRC)

# Rule for generating the lex output
$(LEX_OUTPUT): $(LEX_SRC) $(YACC_HEADER)
	$(LEX) -o $(LEX_OUTPUT) $(LEX_SRC)

# Rule for compiling lex and yacc output files (ignore warnings as errors)
$(BUILD_DIR)/lex.yy.o: $(LEX_OUTPUT)
	$(CC) $(CFLAGS) $(LEXFLAGS) -c $< -o $@

$(BUILD_DIR)/mom.tab.o: $(YACC_OUTPUT)
	$(CC) $(CFLAGS) $(LEXFLAGS) -c $< -o $@

$(BUILD_DIR)/pdf.o: $(SRC_DIR)/pdf.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/pdf.c -o $(BUILD_DIR)/pdf.o

# Rule for compiling the main C file
$(BUILD_DIR)/throff.o: $(SRC_DIR)/throff.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/throff.c -o $(BUILD_DIR)/throff.o

# Clean up generated files
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean
