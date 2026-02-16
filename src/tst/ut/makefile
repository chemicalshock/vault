#
# makefile for shocktest unit tests
#
UT_MAKEFILE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
REPO_ROOT := $(abspath $(UT_MAKEFILE_DIR)/../../..)
# Include moved to bottom so test targets are defined before rule generation
# include $(REPO_ROOT)/dep/shocktest/src/tst/ut/makefile.shocktest.mk

# ------------------------------
# SUT targets (none yet)
# ------------------------------
# X86CPPTARGETSOL += example_SUT.cpp.so
# example_SUT.cpp.so,SRCS := $(REPO_ROOT)/src/lib/example.cpp
# example_SUT.cpp.so,DEPS :=
# example_SUT.cpp.so,CPPFLAGS = $(GLOBAL_CPPFLAGS)

# ------------------------------
# Test targets
# ------------------------------
X86CPPTARGET += TEST_VAULT
TEST_VAULT,SRCS := test_vault.cpp
TEST_VAULT,USRLIBS := 
TEST_VAULT,CPPFLAGS = $(GLOBAL_CPPFLAGS)
TEST_VAULT,LDFLAGS :=

# Include shocktest makefile from dep/shocktest symlink (no absolute path).
include $(REPO_ROOT)/dep/shocktest/src/tst/ut/makefile.shocktest.mk



