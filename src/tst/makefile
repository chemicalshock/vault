# -------------------------------------------------------------
# Unit Test Build System (for src/tst)
# -------------------------------------------------------------
# Builds all SUTs and test binaries into ./bld/
# Usage: make, make tests, make run, make clean
# -------------------------------------------------------------

.DEFAULT_GOAL := tests
ROOT_DIR := $(abspath ../..)
BUILD_DIR := $(CURDIR)/bld

# Compiler config
CXX ?= g++
GLOBAL_CPPFLAGS := -I$(ROOT_DIR)/src/inc -I$(ROOT_DIR)/dep
CXXFLAGS := -std=c++17 -Wall -Wextra -Wno-unknown-pragmas
LDFLAGS :=

# ==============================
#  SUT Definitions
# ==============================
X86CPPTARGETSOL ?=

# ==============================
#  Test Definitions
# ==============================
X86CPPTARGET ?=

X86CPPTARGET += TEST_VAULT
TEST_VAULT,SRCS := test_vault.cpp
TEST_VAULT,USRLIBS := 
TEST_VAULT,CPPFLAGS := $(GLOBAL_CPPFLAGS)
TEST_VAULT,LDFLAGS :=

# include makeshift
include $(ROOT_DIR)/dep/makeshift/makeshift.mk