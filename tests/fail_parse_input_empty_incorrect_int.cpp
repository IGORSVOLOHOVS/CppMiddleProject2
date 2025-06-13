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
        constexpr fixed_string source_2("I want to show 123bc");
        constexpr fixed_string source_5("I want to show 1abc2");
        
        constexpr auto input_2 = parse_input<0, fmt, source_2, int>(); // throw "This number has unother symbols.\n";
        constexpr auto input_5 = parse_input<0, fmt, source_5, int>(); // throw "This number has unother symbols.\n";

    }
}