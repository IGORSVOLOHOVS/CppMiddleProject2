#include "format_string.hpp"
#include "parse.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <cassert>
#include <print>

using namespace stdx::details;

using namespace std::string_view_literals;
int main() { 
    // Test 10: incorrect number format
    {
        constexpr auto fmt = "I want to show {%d}"_fs;
        constexpr fixed_string source_1("I want to show a123bc");
        constexpr fixed_string source_4("I want to show abc");
        
        constexpr auto input_1 = parse_input<0, fmt, source_1, int>(); // throw "This is not a number.\n";
        constexpr auto input_4 = parse_input<0, fmt, source_4, int>(); // throw "This is not a number.\n";

    }
}