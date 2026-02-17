# --------------------------------------------------------------------
#
# !\file makefile.project
# !\brief Makefile for project specifics.
# !\author Colin J.D. Stewart
#
# --------------------------------------------------------------------
#             Copyright (c) 2026. Colin J.D. Stewart
#                     All rights reserved
# --------------------------------------------------------------------

#
# project identification
#
PROJECT ?= vault

BIN_NAME ?= $(PROJECT)
LIB_BASENAME ?= $(PROJECT)

#
# build configuration
#
BUILD_EXE ?= 1
BUILD_STATIC ?= 1
BUILD_SHARED ?= 0

# c++ version
CXX_STD ?= c++20
MODE ?= debug

#
# extensions
#
USER_CPPFLAGS :=
USER_CXXFLAGS :=
USER_LDFLAGS :=
USER_LDLIBS :=

#
# include makeshift
#
include ../makeshift/makeshift.mk