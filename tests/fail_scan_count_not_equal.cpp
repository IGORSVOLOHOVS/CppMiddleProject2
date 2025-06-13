#include "format_string.hpp"
#include "parse.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <cassert>
#include <print>

using namespace stdx::details;

using namespace std::string_view_literals;
int main() { 
    // Test 12: no number for placeholder
    {
        constexpr auto fmt = "I want to sum {} and {%d} numbers."_fs;
        constexpr fixed_string source("I want to sum 123 and -4567 numbers.");

        constexpr auto scan_res = stdx::scan<fmt, source, std::string_view>(); // throw "Compile error: The count of types are not the same as count of placeholders";

    }

    return 0;
}
