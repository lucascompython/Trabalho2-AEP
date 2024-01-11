ifeq ($(OS),Windows_NT)
all:
	@powershell.exe -NoProfile -ExecutionPolicy ByPass -File make.ps1
release:
	@powershell.exe -NoProfile -ExecutionPolicy ByPass -File make.ps1 -release
clean:
	@powershell.exe -NoProfile -ExecutionPolicy ByPass -File make.ps1 -clean
run:
	@powershell.exe -NoProfile -ExecutionPolicy ByPass -File make.ps1 -run
else

# Compiler and flags
CC = clang
CFLAGS = -Wall -Wextra -Werror -std=c17 -I./external/yyjson/src # Mostrar todos os avisos e tratá-los como erros, usar C17 e incluir yyjson
LDFLAGS = -luuid -lm # para uuid_gen e math.h

# Compiler flags for the release build
CFLAGS_RELEASE = -O2 -DNDEBUG -I./external/yyjson/src
LDFLAGS_RELEASE = -s -lm -luuid

# Directories
SRC_DIR = src
BUILD_DIR = build

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c) external/yyjson/src/yyjson.c # Todos os ficheiros .c na pasta src e o yyjson.c 
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Executable name
TARGET = gestao_stocks

# Build the executable
all: $(BUILD_DIR)/$(TARGET)

# Build the release version with optimizations
release: CFLAGS = $(CFLAGS_RELEASE)
release: LDFLAGS = $(LDFLAGS_RELEASE)
release: all

# Build the executable and run it
run: all
	./$(BUILD_DIR)/$(TARGET)

# Link object files to create the executable
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean the build directory
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all release clean

endif
