# cpp-middle-project2: software requirements specification

Reverse-engineered from commit `c67878c` on 2026-11-18. Only behaviour present in the code is stated as a requirement. Line references to headers are to the files as of that commit.

## 1. Purpose and scope

The library extracts typed values from a source string according to a format string during compilation, the reverse of `std::format` (`README.md`).

In scope:

- the four headers in `include/` (namespace `stdx`, `stdx::details`);
- the CTest suite in `tests/` and the refusal checker in `cmake/expect_compile_refusal.cmake`.

Out of scope:

- run-time parsing: every entry point is `consteval` or used in constant expressions;
- types other than integers and types convertible to `std::string_view` (`include/parse.hpp:93-96`);
- installation: `recipe.ps1 install` does nothing (`docs/install.md`).

## 2. Context

| Item | Description | Source |
| --- | --- | --- |
| Actor: C++ developer | includes `scan.hpp` and calls `stdx::scan` | `docs/manual.md` |
| Compiler | evaluates the library; a refused `throw` is the error channel | `README.md` |
| Toolchain (lab) | mingw g++ ≥ 13, CMake ≥ 3.25, Ninja, LLVM | `docs/install.md` |
| Toolchain (container) | Ubuntu, g++-14, cmake, make, clangd-19 | `.devcontainer/Dockerfile` |
| Toolchain (MSVC) | cl.exe x64 through `windows-msvc-*` presets | `CMakePresets.json`, `scripts/build_windows.ps1` |
| Lab share | target of `recipe.ps1 publish` via `LAB_SHARE` | `recipe.ps1` |

No network, file system or hardware access happens in the library.

## 3. Functional requirements

### 3.1 fixed_string

- **FR-1** The system shall provide `fixed_string<N>`, a structural type holding a `std::array<char, N>`, constructible from a string literal with deduction of `N`. Source: `include/types.hpp:14-16`, `59`, `63-64`. Verification: `true_fixed_string`.
- **FR-2** The system shall report `size()` as the number of characters before the first `'\0'`. Source: `include/types.hpp:34-37`. Verification: `true_fixed_string`.
- **FR-3** The system shall build a substring from a `[from, to)` character range, copying at most `N` characters, and throw `"Invalid range"` if `from > to`. Source: `include/types.hpp:25-32`, `45`. Verification: `true_fixed_string` (valid range only); invalid range: no test.
- **FR-4** The system shall compare a `fixed_string` with another `fixed_string` over the whole array, and with a string literal by size and content. Source: `include/types.hpp:47-57`. Verification: `true_fixed_string`.

### 3.2 Format string

- **FR-5** The system shall provide the literal `"..."_fs` that yields `format_string<str>`. Source: `include/format_string.hpp:107-110`. Verification: `true_format_string`.
- **FR-6** The system shall count placeholders `{}`, `{%d}`, `{%u}`, `{%s}` at compile time and expose the count as `number_placeholders`. Source: `include/format_string.hpp:14-74`, `103`. Verification: `true_format_string`.
- **FR-7** The system shall return an error from the count for an empty format string, an unclosed placeholder, a specifier other than `d`, `u`, `s`, or a missing `}`, and calling `.value()` on that error shall stop compilation. Source: `include/format_string.hpp:16-18`, `32-34`, `43-45`, `59-61`, `66-68`, `103`. Verification: `fail_unknown_placeholder` (invalid specifier only).
- **FR-8** The system shall record, for each placeholder, the index of its `{` and its `}` in `placeholder_positions`. Source: `include/format_string.hpp:76-99`, `104`. Verification: `true_format_string`.

### 3.3 Value parsing

- **FR-9** The system shall parse an integral `T` (cv-qualifiers removed) with `std::from_chars`, and throw on empty input, a non-number, an out-of-range value, or trailing characters. Source: `include/parse.hpp:64-83`. Verification: `true_parse_value`, `fail_parse_input_empty_int`, `fail_parse_input_not_int`, `fail_no_numer_for_placefolder`, `fail_parse_input_empty_incorrect_int`; out of range: no test.
- **FR-10** The system shall construct a `T` convertible to `std::string_view` directly from the input characters. Source: `include/parse.hpp:85-91`. Verification: `true_parse_value`.
- **FR-11** The system shall throw `"Unsupported type"` for any other `T`. Source: `include/parse.hpp:93-96`. Verification: `fail_parse_input_unknown_type`.

### 3.4 Source slicing and placeholder input

- **FR-12** The system shall compute the `[start, end)` range of the `I`-th value in the source. The first value starts at the format's first `{` index. A later value starts after the literal separator found in the source after the previous value. A value ends at the next separator or at the end of the source. Source: `include/parse.hpp:17-61`. Verification: `true_get_current_source_for_parsing`.
- **FR-13** The system shall reject an index `I` outside `[0, number_placeholders)` with a `static_assert`. Source: `include/parse.hpp:18`. Verification: no test.
- **FR-14** The system shall parse the `I`-th value as `T` through `parse_value`, choosing the branch from `get_placeholder_type<fmt>()`, and throw `"Invalid placeholder for the given type, or type mismatch."` if no branch returns. Source: `include/parse.hpp:105-142`. Verification: `true_parse_input`; the throw: no test. See limitation 1 in [docs/README.md](docs/README.md#known-limitations-and-problems): the branch is chosen from the whole format string's text.

### 3.5 scan

- **FR-15** The system shall provide `stdx::scan<fmt, source, Ts...>()` (consteval) returning `scan_result<Ts...>` with one parsed value per placeholder. Source: `include/scan.hpp:13-26`. Verification: `true_scan`, `scan_tests` (Test 5, Test 6).
- **FR-16** The system shall throw `"Compile error: The count of types are not the same as count of placeholders"` when `sizeof...(Ts)` differs from `number_placeholders`. Source: `include/scan.hpp:14-16`. Verification: `fail_scan_count_not_equal`.
- **FR-17** The system shall give `scan_result` a constexpr `get<I>()` and `size()` equal to `sizeof...(Ts)`. Source: `include/types.hpp:69-79`. Verification: `true_scan`, `true_scan_no_plachold`.

### 3.6 Test harness

- **FR-18** The build shall stop at configure time when a `tests/fail_<name>.cpp` has no `refusal_<name>` entry (non-MSVC). Source: `CMakeLists.txt`, `message(FATAL_ERROR "${name} has no refusal_${name}...")`. Verification: no test.
- **FR-19** The refusal check shall pass only if g++ refuses the file, the count of `'<throw-expression>' is not a constant expression` errors equals `REFUSALS`, and every echoed throwing line contains `REASON`. Source: `cmake/expect_compile_refusal.cmake`. Verification: the 7 `fail_*` CTest entries.
- **FR-20** The repository shall follow the lab tooling of repo-quality-template; `recipe.ps1 test` also requires exactly 10 `throw|try|catch` lines in the four headers (`$ThrowLinesOnPurpose`). Source: `recipe.ps1`, `README.md`. Verification: `recipe.ps1 test`.

### 3.7 Declared but not implemented

| Item | Source | Note |
| --- | --- | --- |
| Per-placeholder type checking by `{%d}` / `{%u}` / `{%s}` | `README.md` ("Placeholders"), `docs/manual.md` ("`{%d}` a signed integer, `{%u}` an unsigned integer…") | `get_placeholder_type` reads the whole format string, not the placeholder (`include/parse.hpp:105-115`); inference from reading, not compiled |
| "The text between placeholders must match the source" | `docs/manual.md` | the prefix before the first placeholder is never compared, and a missing separator is not an error (`include/parse.hpp:30-31`, `42-43`, `57-58`); inference from reading |
| Manual tests | `tests/manual/tests.json` | list is empty |
| Test cases 8-12 in `scan_tests` | `tests/main.cpp:170-222` | bodies commented out, they only print `[OK]`; covered by `fail_*.cpp` instead |

## 4. Interfaces

Public API (C++ headers):

| Entity | Signature (abridged) | Header |
| --- | --- | --- |
| `stdx::scan` | `template <format_string fmt, fixed_string source, typename... Ts> consteval scan_result<Ts...> scan()` | `include/scan.hpp:23-26` |
| `stdx::details::fixed_string<N>` | ctor from `const char(&)[N]`, from `(const char*, const char*)`; `size()`, `view()`, `operator[]`, `substr(from, to)`, `operator==` | `include/types.hpp:14-60` |
| `stdx::details::format_string<str>` | `fmt`, `number_placeholders`, `placeholder_positions` | `include/format_string.hpp:12-105` |
| `operator""_fs` | `template <fixed_string str> constexpr auto operator""_fs()` | `include/format_string.hpp:107-110` |
| `stdx::details::parse_value<T, input>` | integral / string-view-convertible / other | `include/parse.hpp:64-96` |
| `stdx::details::parse_input<I, fmt, source, T>` | parses one placeholder | `include/parse.hpp:118-142` |
| `stdx::details::scan_result<Ts...>` | `get<I>()`, `size()` | `include/types.hpp:69-79` |

Format string grammar (`include/format_string.hpp:24-71`): literal characters, and placeholders `{` [`%` (`d`|`u`|`s`)] `}`.

Compile-time error messages (the `throw` lines):

| Message | Source | Test |
| --- | --- | --- |
| `Input is empty. Parsing error.` | `include/parse.hpp:69` | `fail_parse_input_empty_int` (1 refusal) |
| `This is not a number.` | `include/parse.hpp:74` | `fail_parse_input_not_int` (2), `fail_no_numer_for_placefolder` (1) |
| `This number is larger than an std::remove_cv_t<T>.` | `include/parse.hpp:76` | no test |
| `This number has unother symbols.` | `include/parse.hpp:78` | `fail_parse_input_empty_incorrect_int` (2) |
| `Parsing error!` | `include/parse.hpp:80` | no test |
| `Unsupported type` | `include/parse.hpp:95` | `fail_parse_input_unknown_type` (1) |
| `Invalid placeholder for the given type, or type mismatch.` | `include/parse.hpp:141` | no test |
| `Compile error: The count of types are not the same as count of placeholders` | `include/scan.hpp:15` | `fail_scan_count_not_equal` (1) |
| `'}' hasn't been found in appropriate place` | `include/format_string.hpp:93` | no test |
| `Invalid range` | `include/types.hpp:27` | no test |
| format errors via `std::expected` → `bad_expected_access` | `include/format_string.hpp:103` | `fail_unknown_placeholder` (1) |

Refusal counts are from `CMakeLists.txt` (`refusal_*` lines).

## 5. Data

- `fixed_string<N>`: `std::array<char, N> data`, zero-initialised; content ends at the first `'\0'` (`include/types.hpp:34-37`, `59`).
- `parse_error = fixed_string<43>` (`include/types.hpp:66-67`).
- `placeholder_positions`: `std::array<std::pair<size_t, size_t>, number_placeholders>` (`include/format_string.hpp:77`).
- `PlaceholderType`: `STRING_VIEW = 0`, `UNSIGNED_INT`, `SIGNED_INT` (`std::uint8_t`) (`include/parse.hpp:98-102`).
- `scan_result<Ts...>`: `std::tuple<Ts...> values_` (`include/types.hpp:71`).
- `tests/manual/tests.json`: `{"version": 1, "tests": [...]}`; entry shape checked by `tests/test_manual_list.py`.

Nothing is stored at run time.

## 6. Non-functional requirements

- **NFR-1** C++23, standard required, extensions off. Source: `CMakeLists.txt:4-6`.
- **NFR-2** Header-only, depends only on the standard library: `add_library(scan INTERFACE)`. Source: `CMakeLists.txt` (`add_library`), `README.md`.
- **NFR-3** Test targets build with `-Wall -Wextra -Wpedantic -Werror` (non-MSVC). Source: `CMakeLists.txt`, `warn_as_errors`.
- **NFR-4** `clang-tidy` with `bugprone-*`, `performance-*`, `portability-*`, `clang-analyzer-*`, warnings as errors. Source: `.clang-tidy`, `tests/.clang-tidy`.
- **NFR-5** No locks or synchronisation primitives in `include/`. Source: `recipe.ps1`, `Assert-NoWordInCore 'lock'`.
- **NFR-6** Under MSVC, sources compile with `/utf-8`. Source: `CMakeLists.txt`.

## 7. Constraints and dependencies

| Dependency | Version | Source |
| --- | --- | --- |
| CMake | ≥ 3.25 | `CMakeLists.txt:1`, `CMakePresets.json` |
| g++ (mingw) | ≥ 13 | `docs/install.md` |
| g++ (container) | 14 | `.devcontainer/Dockerfile` |
| Ninja | not pinned | `docs/install.md`, `recipe.ps1` |
| LLVM `clang-tidy` / `clang-format` | not pinned; `tests/.clang-tidy` mentions 21.1.6 | `docs/install.md`, `tests/.clang-tidy` |
| Python + pytest | not pinned (no lock file) | `CMakeLists.txt` (`manual_list`) |
| Base image | `ubuntu@sha256:3afff29d…` | `.devcontainer/Dockerfile` |

No package manager manifest (conanfile, vcpkg.json) exists (`scripts/build_windows.ps1` says so too).

## 8. Acceptance

1. On a machine with the lab toolchain, `.\recipe.ps1 test` and `$LASTEXITCODE` equal `0` (`CLAUDE.md`, rule 3). This covers FR-1…FR-12, FR-14…FR-17, FR-19, FR-20 and NFR-3…NFR-5.
2. `ctest --test-dir build --output-on-failure --no-tests=error` lists `scan_tests`, 7 `true_*`, 7 `fail_*` and `manual_list`, all passing.
3. `.\recipe.ps1 run` prints `All tests passed successfully!` (`tests/main.cpp:224`).
4. Requirements marked "no test" (FR-13, FR-18, parts of FR-3, FR-7, FR-9, FR-14) are not covered by these steps.

## 9. Open questions

- Who uses the library besides the course submission, and is it consumed by other lab projects through `publish`? `$Needs` lists none here.
- Should `{%d}` / `{%u}` / `{%s}` restrict the type per placeholder, as `README.md` and `docs/manual.md` suggest? The code reads the whole format string (§3.7).
- Should a literal prefix or separator that does not match the source be an error?
- Is the MSVC route (`CMakePresets.json`, `scripts/build_windows.ps1`, `check-compile-fails`) still supported? Partly answered by the code: no workflow in `.gitea/workflows/` runs it (they call only `recipe.ps1`, which builds with g++ and Ninja), matching `docs/install.md`. Whether it should be kept is the owner's decision.
- Which g++ version is the minimum that is actually tested: 13 (`docs/install.md`) or 14 (`.devcontainer/Dockerfile`)? Unknown: the workflows run `recipe.ps1` with whatever mingw g++ the runner has, and no version is pinned.
- The nine lab-requirement exceptions in `README.md` point to Gitea issues that are not in this repository.
