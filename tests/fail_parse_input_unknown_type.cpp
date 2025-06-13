#include "format_string.hpp"
#include "parse.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <cassert>
#include <print>

using namespace stdx::details;

using namespace std::string_view_literals;
int main() { 
    // Test 9: parse_input with unknown type
    {
        constexpr auto fmt = "I want to show {}"_fs;
        constexpr fixed_string source("I want to show 12.3");
        
        constexpr auto input = parse_input<0, fmt, source, double>();
    }
}