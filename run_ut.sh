#!/bin/bash
# -------------------------------------------------------------
# !\file run_ut.sh
# !\brief Run all unit tests (Linux/macOS/MSYS2)
# !\author Colin J.D. Stewart
# -------------------------------------------------------------
#             Copyright (c) 2025. Colin J.D. Stewart
#                     All rights reserved
# -------------------------------------------------------------

TEST_DIR="bld/tests"
LOG_FILE="test_log.txt"
PASS=0
FAIL=0

if [ ! -d "$TEST_DIR" ]; then
    echo "[run_ut] Unit test directory not found: $TEST_DIR"
    echo "Please run 'make test' first."
    exit 1
fi

echo "Running unit tests in $TEST_DIR..."
echo "" > "$LOG_FILE"

for f in "$TEST_DIR"/*; do
    if [ -x "$f" ] && [[ ! -d "$f" ]]; then
        echo "[$(basename "$f")]" >> "$LOG_FILE"
        "$f" >> "$LOG_FILE" 2>&1
        if [ $? -eq 0 ]; then
            echo -e "\033[32m[PASS]\033[0m $(basename "$f")"
            PASS=$((PASS+1))
        else
            echo -e "\033[31m[FAIL]\033[0m $(basename "$f")"
            FAIL=$((FAIL+1))
        fi
        echo "" >> "$LOG_FILE"
    fi
done

echo ""
echo "Tests complete. [PASS: $PASS] [FAIL: $FAIL]"
echo "Full log saved to $LOG_FILE"
