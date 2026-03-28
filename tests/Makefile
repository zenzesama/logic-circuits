# =============================================================================
# Makefile - logic simulator unit tests
# Commands:
#   make       - build all tests
#   make run   - build and run all tests
#   make clean - remove compiled binaries
# =============================================================================

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -I..

SRCS = ../gates.c ../logic.c ../flipFlops.c ../print.c ../hacks.c ../ram.c

TESTS = test_gates \
        test_adders \
        test_flipflops \
        test_memory \
        test_selector \
        test_adding_machine \
		test_hacks \
		test_ram

.PHONY: run clean

all: $(TESTS)

test_%: $(SRCS) test_%.c
	$(CC) $(CFLAGS) -o $@ $^

run: all
	@echo "============================================"
	@echo "  Running all tests"
	@echo "============================================"
	@failed=0; \
	for t in $(TESTS); do \
		./$$t || failed=$$((failed + 1)); \
	done; \
	echo "--------------------------------------------"; \
	if [ $$failed -eq 0 ]; then \
		echo "  All test suites passed."; \
	else \
		echo "  $$failed suite(s) had failures."; \
	fi; \
	echo "============================================"

clean:
	rm -f $(TESTS)