#
# makefile for shocktest unit tests
#
UT_MAKEFILE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
REPO_ROOT := $(abspath $(UT_MAKEFILE_DIR)/../../..)

# ------------------------------
# SUT targets (none yet)
# ------------------------------
X86CPPTARGETSOL += example_SUT.cpp.so
example_SUT.cpp.so,SRCS := $(REPO_ROOT)/src/lib/example.cpp
example_SUT.cpp.so,DEPS :=
example_SUT.cpp.so,CPPFLAGS = $(GLOBAL_CPPFLAGS)

# ------------------------------
# Test targets
# ------------------------------
X86CPPTARGET += TEST_EXAMPLE_FUNCTIONS
TEST_EXAMPLE_FUNCTIONS,SRCS = test_example_functions.cpp
TEST_EXAMPLE_FUNCTIONS,USRLIBS = example_SUT.cpp.so
TEST_EXAMPLE_FUNCTIONS,CPPFLAGS = $(GLOBAL_CPPFLAGS) -I$(REPO_ROOT)/dep/shocktest/src/inc
TEST_EXAMPLE_FUNCTIONS,LDFLAGS =


# Include shocktest makefile from dep/shocktest symlink (no absolute path).
include $(REPO_ROOT)/dep/shocktest/src/tst/ut/makefile.shocktest.mk
