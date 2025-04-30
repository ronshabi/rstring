# Copyright (c) 2025, Ron Shabi <ron@ronsh.net>
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.25)
project(RString)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_COLOR_MAKEFILE ON)

add_library(rstring STATIC
    "include/rstring/rstring.h"
    "src/rstring.c"
)

target_include_directories(rstring PUBLIC
    "include"
)