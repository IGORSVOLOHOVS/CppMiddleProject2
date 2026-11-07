# cpp-middle-project2

A compile-time `scan` in modern C++23, the reverse of `std::format`: a format string and a source string go in as template arguments, and typed values come out while the program compiles. It began as the sprint 2 project of the Yandex Practicum "Middle C++ developer" course. It is four headers and depends on nothing but the standard library.

## What it does

- **`stdx::scan<fmt, source, Ts...>()`**: reads `Ts...` out of `source` by the placeholders of `fmt`, and answers a `scan_result` whose `get<I>()` and `size()` are usable in a `static_assert`.
- **Placeholders**: `{}`, `{%d}`, `{%u}` and `{%s}`.
- **`fixed_string` and `format_string`**: a string usable as a template argument, and a format string counted and checked at compile time, through the `_fs` literal.
- **`parse_value<T, input>()`**: every integer type, with their limits, and anything that converts to `std::string_view`.

A bad format string or a value that does not parse stops the build. That is the design: the headers `throw` during constant evaluation, and the compiler refuses the throw. `.\recipe.ps1 test` pins those 10 lines.

## Layout

```text
include/  the library, four headers
tests/    main.cpp and true_*.cpp must compile and run; fail_*.cpp must be refused
cmake/    expect_compile_refusal.cmake, which reads why a fail_*.cpp was refused
```

## Build and test

`.\recipe.ps1 test` builds in Debug, because the tests hold an `assert`, then runs `ctest`, `clang-tidy` and `clang-format`, and searches `include` for locks and for `throw`, `try` and `catch`. It needs mingw g++, cmake, ninja and LLVM. `ctest` compiles each `tests/fail_*.cpp`, and `CMakeLists.txt` names how many refusals each one must give and the throw each must come from: a refusal for any other reason fails the test. `docs/install.md` has the details, and `docs/manual.md` covers the library.

MIT licensed: `LICENSE` is the course's, copyright Yandex Practicum.

## Lab requirements

This repository complies with [revision 4.0.0](http://127.0.0.1:3000/igors-lab/software-requirements/src/tag/v4.0.0/BEST_REQUIREMENTS.md) of the lab requirements, except R-02 (#8), R-09 (#9), R-12 (#10), R-13 (#11), R-13 (#12), R-14 (#13), R-37 (#14), R-41 (#15), R-42 (#16).
