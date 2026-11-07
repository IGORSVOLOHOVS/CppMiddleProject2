# Install

The library needs no installing: put `include` on your include path and build with C++23.

To build and test it here:

1. Install mingw g++ 13 or newer, CMake 3.25 or newer, Ninja, and LLVM for `clang-tidy` and `clang-format`.
2. Clone this repository.
3. Run `.\recipe.ps1 test`.

`recipe.ps1` looks for g++ in `C:\ProgramData\mingw64\mingw64\bin` and for LLVM in `C:\Program Files\LLVM\bin`, and adds those folders to its own PATH.

`.\recipe.ps1 build` compiles the tests into `build\`. `.\recipe.ps1 install` does nothing: there is no installer.

`CMakePresets.json`, `scripts/build_windows.ps1`, `cmake/expect_compile_failure.cmake`, `.devcontainer` and `.vscode` are what the course and the old GitHub route left. No gate runs them. Under MSVC, `CMakeLists.txt` keeps the old `check-compile-fails` target, which sees only that the compiler refused a file, not why.
