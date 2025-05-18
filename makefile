# -------------------------------------------------------------
# !\file makefile
# !\brief Generic makefile for various projects
# !\author Colin J.D. Stewart
# -------------------------------------------------------------
#             Copyright (c) 2025. Colin J.D. Stewart
#                     All rights reserved
# -------------------------------------------------------------

# Optional dependency includes
-include dep.gen.mk

# Target names
BIN_NAME      = program
LIB_NAME      = libname.so.1

# Compiler settings
CXX       = g++
CXXFLAGS  = -O3 -DdENABLE_TRACE -g -std=c++17 -Wall -I./src/inc -I./dep $(DEP_INC_FLAGS) -MMD -MP
LDFLAGS   = -Wl,--no-as-needed -Wl,--as-needed -Wl,-O1 -Wl,--as-needed -Wl,--strip-all

# Directory layout
SRC_DIR   = src
BIN_DIR   = $(SRC_DIR)/bin
LIB_DIR   = $(SRC_DIR)/lib
TST_DIR   = $(SRC_DIR)/tst
INC_DIR   = $(SRC_DIR)/inc
OBJ_DIR   = $(SRC_DIR)/bld
OUT_DIR   = bld

# Output targets
EXEC_OUT  = $(OUT_DIR)/$(BIN_NAME)
LIB_OUT   = $(OUT_DIR)/$(LIB_NAME)

# Source files
BIN_SOURCES  = $(wildcard $(BIN_DIR)/*.cpp)
#LIB_SOURCES := $(wildcard $(LIB_DIR)/*.cpp) $(wildcard $(LIB_DIR)/*/*.cpp) $(wildcard $(LIB_DIR)/*/*/*.cpp)

LIB_SOURCES := \
  $(shell find $(LIB_DIR) -type f \( -name '*.cpp' -o -name '*.c' \)) \
  $(shell find dep -type f \( -name '*.cpp' -o -name '*.c' \))

LIB_SOURCES += $(wildcard $(LIB_DIR)/*.S)  


# Object files
BIN_OBJECTS  = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(BIN_SOURCES))
LIB_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(filter %.cpp,$(LIB_SOURCES)))
LIB_OBJECTS += $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(filter %.c,$(LIB_SOURCES)))


# ==============================
# Default Target
# ==============================

.DEFAULT_GOAL := all

all: $(EXEC_OUT) $(LIB_OUT)

# ==============================
# Executable
# ==============================

$(EXEC_OUT): $(BIN_OBJECTS) $(LIB_OBJECTS) | $(OUT_DIR)
	@echo Linking executable $@
	$(CXX) $(CXXFLAGS) $^ -o $@  $(LDFLAGS)

# ==============================
# LIB Target
# ==============================

$(LIB_OUT): $(LIB_OBJECTS) | $(OUT_DIR)
	@echo Linking shared library $@
	$(CXX) -shared $(CXXFLAGS) -fPIC $^ -o $@

# ==============================
# Compilation Rule
# ==============================

# Rule for sources in src/bin (VM executable); no -fPIC.
$(OBJ_DIR)/bin/%.o: $(BIN_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling binary: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for sources in src/lib (DLL/shared library); with -fPIC.
$(OBJ_DIR)/lib/%.o: $(LIB_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling library: $<"
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@

# C file compilation (used for deps)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling C: $<"
	$(CXX) $(CXXFLAGS) -x c -fPIC -c $< -o $@

# S files
$(OBJ_DIR)/lib/%.o: $(LIB_DIR)/%.S
	@mkdir -p $(dir $@)
	@echo "Assembling: $<"
	$(CXX) -c $< -o $@	


# ==============================
# Unit Tests
# ==============================

tests:
	@cd $(TST_DIR) && make tests

run-tests:
	@cd $(TST_DIR) && make run

uts:
	@cd $(TST_DIR) && make && make run

clean-tests:
	@cd $(TST_DIR) && make clean

# ==============================
# Clean Targets
# ==============================

clean-all:
	@echo Cleaning main project output...
	@rm -rf $(OBJ_DIR)
	@rm -f $(EXEC_OUT)
	@rm -f $(DLL_OUT)
	@rm -f $(OUT_DIR)/$(HEADER_PUBLIC)

clean: clean-all clean-tests

# ==============================
# Meta
# ==============================

.PHONY: all tests uts run-tests clean-tests clean-all clean

DEPS := $(BIN_OBJECTS:.o=.d) $(LIB_OBJECTS:.o=.d)
-include $(DEPS)
