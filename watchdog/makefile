# Compiler and flags
CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -fPIC

# Directories
BASE_DIR = /home/gigigu/amit.ellenbogen/projects
SRC_DIR = $(BASE_DIR)/src
INCLUDE_DIR = $(BASE_DIR)/include
BUILD_DIR = $(BASE_DIR)/bin
LIB_DIR = $(BASE_DIR)/lib
TEST_DIR = $(BASE_DIR)/test
OBJ_DIR = $(BASE_DIR)/obj

RELEASE_LIB_DIR = $(BUILD_DIR)/release/sharedlibs
DEBUG_LIB_DIR = $(BUILD_DIR)/debug/sharedlibs

# Files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Library and dependencies
LIB_NAME = watch_dog
DEPENDENCIES = dvector heap_pq heap scheduler_v2 task uid
LIB_DEPENDENCIES = $(foreach lib,$(DEPENDENCIES),-l$(lib))

# RPATH settings
RPATH = $(LIB_DIR):$(RELEASE_LIB_DIR):$(DEBUG_LIB_DIR)
RPATH_FLAGS = -Wl,--disable-new-dtags -Wl,-rpath,$(LIB_DIR) -Wl,-rpath,$(RELEASE_LIB_DIR) -Wl,-rpath,$(DEBUG_LIB_DIR)

# Targets
SHARED_LIB = $(LIB_DIR)/lib$(LIB_NAME).so
MAIN_EXEC = $(BUILD_DIR)/$(LIB_NAME)_main_exec
TEST_EXEC = $(BUILD_DIR)/$(LIB_NAME)_test
DEBUG_TEST_EXEC = $(BUILD_DIR)/$(LIB_NAME)_test_debug

# Default target
all: $(SHARED_LIB) $(MAIN_EXEC) $(TEST_EXEC) $(DEBUG_TEST_EXEC)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

# Compile test files
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

# Build shared library
$(SHARED_LIB): $(OBJ_FILES)
	@mkdir -p $(LIB_DIR)
	@echo "Building shared library lib$(LIB_NAME).so..."
	@$(CC) -shared -o $@ $^ -lm

# Build main executable
$(MAIN_EXEC): $(OBJ_DIR)/$(LIB_NAME)_exec.o $(SHARED_LIB)
	@mkdir -p $(BUILD_DIR)
	@echo "Building executable $(LIB_NAME)_exec..."
	@$(CC) $(CFLAGS) -o $@ $< -I$(INCLUDE_DIR) -L$(LIB_DIR) -L$(RELEASE_LIB_DIR) -L$(DEBUG_LIB_DIR) -l$(LIB_NAME) $(LIB_DEPENDENCIES) $(RPATH_FLAGS) -lm

# Build test executable
$(TEST_EXEC): $(OBJ_DIR)/$(LIB_NAME)_test_client.o $(SHARED_LIB)
	@mkdir -p $(BUILD_DIR)
	@echo "Building test executable $(LIB_NAME)_test..."
	@$(CC) $(CFLAGS) -o $@ $< -I$(INCLUDE_DIR) -L$(LIB_DIR) -L$(RELEASE_LIB_DIR) -L$(DEBUG_LIB_DIR) -l$(LIB_NAME) $(LIB_DEPENDENCIES) $(RPATH_FLAGS) -lm

# Build debug test executable
$(DEBUG_TEST_EXEC): $(OBJ_DIR)/$(LIB_NAME)_test_client.o $(SHARED_LIB)
	@mkdir -p $(BUILD_DIR)
	@echo "Building debug test executable $(LIB_NAME)_test_debug..."
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $@ $< -I$(INCLUDE_DIR) -L$(LIB_DIR) -L$(RELEASE_LIB_DIR) -L$(DEBUG_LIB_DIR) -l$(LIB_NAME) $(LIB_DEPENDENCIES) $(RPATH_FLAGS) -lm

# Clean target
clean:
	@echo "Cleaning up watch_dog files..."
	@rm -f $(LIB_DIR)/libwatch_dog.so
	@rm -f $(BUILD_DIR)/watch_dog_main_exec
	@rm -f $(BUILD_DIR)/watch_dog_test
	@rm -f $(BUILD_DIR)/watch_dog_test_debug
	@rm -f $(OBJ_DIR)/watch_dog*.o

.PHONY: all clean

