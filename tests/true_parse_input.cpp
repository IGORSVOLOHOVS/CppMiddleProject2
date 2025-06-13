#include "format_string.hpp"
#include "parse.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <cassert>
#include <print>

using namespace stdx::details;

using namespace std::string_view_literals;
int main() { 
    // Test 5: parse_input и scan
    {
        constexpr auto fmt = "I want to sum {} and {%d} numbers."_fs;
        constexpr fixed_string source("I want to sum 123 and -4567 numbers.");
        
        constexpr auto input_1_prs = parse_input<0, fmt, source, std::string_view>();
        constexpr auto input_2_prs = parse_input<1, fmt, source, int16_t>();
        static_assert(input_1_prs == "123");
        static_assert(input_2_prs == -4567);

    }
}