# Copyright (c) 2025, Ron Shabi <ron@ronsh.net>
# SPDX-License-Identifier: MIT

# Note: The actual build system is CMake, this is just for QOL utilities, like
# running clang-format on all relevent code files.

SOURCES = $(shell find src -type f -name '*.[ch]')
HEADERS = $(shell find include -type f -name '*.[ch]')
TESTS   = $(shell find test -type f -name '*.[ch]')

.PHONY: fmt

fmt:
	@clang-format -i --verbose $(SOURCES) $(HEADERS) $(TESTS)

.PHONY: tidy
tidy:
	@clang-tidy -p build --use-color $(SOURCES) $(HEADERS) $(TESTS)