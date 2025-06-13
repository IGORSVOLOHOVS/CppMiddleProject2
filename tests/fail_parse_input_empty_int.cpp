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
        constexpr fixed_string source_3("I want to show ");
        
        constexpr auto input_3 = parse_input<0, fmt, source_3, int>(); // throw "Input is empty. Parsing error.\n";

    }
}