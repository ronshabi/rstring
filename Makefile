# Copyright (c) 2025, Ron Shabi <ron@ronsh.net>
# SPDX-License-Identifier: MIT

# Note: The actual build system is CMake, this is just for QOL utilities, like
# running clang-format on all relevent code files.

.PHONY: fmt

fmt:
	find include src test -type f -name '*.[ch]' -exec clang-format -i --verbose {} +