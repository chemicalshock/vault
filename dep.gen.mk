# -------------------------------------------------------------
#
# !\file dep.gen.mk
# !\brief Auto-generated include flags for all dep/* projects
# !\author Colin J.D. Stewart
#
# -------------------------------------------------------------
#
#             Copyright (c) 2025. Colin J.D. Stewart
#                     All rights reserved
#
# -------------------------------------------------------------

# Scan for header and source folders in dep/*
DEP_INC_DIRS := $(wildcard dep/*/src/inc)
DEP_SRC_DIRS := $(wildcard dep/*/src)
DEP_ALL_INCLUDES := $(DEP_INC_DIRS) $(DEP_SRC_DIRS)

DEP_INC_FLAGS := $(foreach dir, $(DEP_ALL_INCLUDES), -I$(dir))
