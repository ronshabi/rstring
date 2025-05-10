# Copyright (c) 2025, Ron Shabi <ron@ronsh.net>
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.25)
project(RString)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_C_STANDARD 99)

add_library(rstring STATIC
    "include/rstring/rstring.h"
    "src/rstring.c"
)

target_include_directories(rstring PUBLIC
    "include"
)

target_compile_options(rstring PRIVATE
    "-Wall"
    "-Wextra"
    "-Wformat"
    "-Werror"
)

# Add AddressSanitizer to library
target_compile_options(rstring PRIVATE "-fsanitize=address")
target_link_libraries(rstring PRIVATE "-fsanitize=address")

enable_testing()
add_executable(t1-simple-usage "test/t1-simple-usage.c")
target_link_libraries(t1-simple-usage PRIVATE rstring)
add_test(NAME t1-simple-usage COMMAND t1-simple-usage)

add_executable(t2-find "test/t2-find.c")
target_link_libraries(t2-find PRIVATE rstring)
add_test(NAME t2-find COMMAND t2-find)