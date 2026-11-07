# Manual

## The library

Everything happens at compile time, so both strings are template arguments:

```cpp
#include "scan.hpp"

using namespace stdx::details;

constexpr auto fmt = "I want to sum {} and {%d} numbers."_fs;
constexpr fixed_string source("I want to sum 123 and -4567 numbers.");

constexpr auto result = stdx::scan<fmt, source, std::string_view, int16_t>();

static_assert(result.get<0>() == "123");
static_assert(result.get<1>() == -4567);
static_assert(result.size() == 2);
```

`{}` takes the type you name, `{%d}` a signed integer, `{%u}` an unsigned integer and `{%s}` a string view. The text between placeholders must match the source, and it is what separates the values.

## What stops the build

Each of these is a `throw` reached during constant evaluation, so the compiler refuses the program and shows the throwing line:

| what is wrong | what the compiler shows |
| --- | --- |
| the value is not a number | `This is not a number.` |
| the number has more characters after it | `This number has unother symbols.` |
| the value is empty | `Input is empty. Parsing error.` |
| the number does not fit the type | `This number is larger than an std::remove_cv_t<T>.` |
| the type is neither an integer nor a string view | `Unsupported type` |
| the count of types differs from the count of placeholders | `Compile error: The count of types are not the same as count of placeholders` |
| the format string is malformed, such as `{%f}` | `bad_expected_access`, from `value()` on the error of `get_number_placeholders` |

## The tests

`.\recipe.ps1 run` builds and runs `scan_tests`, which prints one line for each of its cases. Most of what it proves is `static_assert`, so a wrong value stops the build before it runs.

`tests/fail_*.cpp` cannot be run: `ctest` compiles each one through `cmake/expect_compile_refusal.cmake`. A new `fail_*.cpp` needs a `refusal_<name>` line in `CMakeLists.txt`, or the configure stops.
