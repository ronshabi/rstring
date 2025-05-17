# RString: fast, lightweight, dynamic strings in C

RString is a lightweight C library providing a dynamic string data structure and related routines, making string processing manipulation quick, efficient and easy! 🧵

> [!CAUTION]
>
> Library is still in alpha stage, Do **not** use this code in production.

### Goals

- Provide modern and efficient implementations of string processing routines found in other languages / standard-libraries, which the standard C library misses.

- Eliminame the need to rewrite a dynamic string structure for each new C project.

- Be easy to integrate in heavily constrained environments (e.g. _embedded_).

### Features

- Tiny footprint 🪶.
- Reentrant design.
- Readable codebase.

### API

Functions are fully documented next to their declarations in doxygen format, here is an overview of the current API:

- Initialization: `rstring_init`

- Cleanup & Memory Management: `rstring_free`, `rstring_ensure_capacity`

- Modification: `rstring_push`, `rstring_push_byte`, `rstring_push_str`, `rstring_clear`

- Comparison: `rstring_cmp`, `rstring_cmp_ignore_case`, `rstring_cmp_str`, `rstring_cmp_str_ignore_case`, `rstring_equals`, `rstring_equals_ignore_case`, `rstring_equals_str`, `rstring_equals_str_ignore_case`

- Search: `rstring_find_first`, `rstring_find_first_str` `rstring_find_first_str_ignore_case`, `rstring_find_first_byte`, `rstring_find_last_byte`
- Utility: `rstring_is_empty`

## Building

This library uses CMake. A static build is configured by default.

```sh
mkdir build && cd build
cmake ..
make
```

## Development

Quality of life developer utilities can be found in the root directory's `Makefile`:

- `make fmt`: run clang-format on all source & test files.
- `make tidy`: run clang-tidy on all source & test files.

## Author

Ron Shabi <<ron@ronsh.net>>

## License

MIT
