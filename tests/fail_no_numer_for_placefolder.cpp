#include "format_string.hpp"
#include "parse.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <cassert>
#include <print>

using namespace stdx::details;

using namespace std::string_view_literals;
int main() { 
    // Test 11: no number for placeholder
    {
        constexpr auto fmt = "I want to show {%d}"_fs;
        constexpr fixed_string source("I want to show    ");
        
        constexpr auto input = parse_input<0, fmt, source, int>(); // throw "This is not a number.\n";

    }
}