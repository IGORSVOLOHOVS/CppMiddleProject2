#include "format_string.hpp"
#include "parse.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <cassert>
#include <print>

using namespace stdx::details;

using namespace std::string_view_literals;
int main() { 
    // Test 3: get_current_source_for_parsing
    {
        constexpr auto fmt = "I want to sum {} and {%d} numbers."_fs;
        constexpr fixed_string source("I want to sum 123 and -4567 numbers.");

        constexpr auto source_1 = get_current_source_for_parsing<0, fmt, source>();
        constexpr auto source_2 = get_current_source_for_parsing<1, fmt, source>();

        static_assert(source_1.first == 14, "Parsed source 1 start position incorrect");
        static_assert(source_1.second == 17, "Parsed source 1 end position incorrect");
        static_assert(source_2.first == 22, "Parsed source 2 start position incorrect");
        static_assert(source_2.second == 27, "Parsed source 2 end position incorrect");

        static_assert(source.substr(source_1.first, source_1.second) == "123", "Parsed source 1 content mismatch");
        static_assert(source.substr(source_2.first, source_2.second) == "-4567", "Parsed source 2 content mismatch");

    }
}