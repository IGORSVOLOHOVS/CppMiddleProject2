#include "format_string.hpp"
#include "parse.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <cassert>
#include <print>

using namespace stdx::details;

using namespace std::string_view_literals;

int main() { 
    // Test 2: format_string
    {
        constexpr auto fmt = "I want to sum {} and {%d} numbers."_fs;

        static_assert(fmt.fmt == "I want to sum {} and {%d} numbers.", "Format string content mismatch");
        static_assert(fmt.number_placeholders == 2, "Incorrect number of placeholders");
        static_assert(fmt.placeholder_positions.at(0).first == 14, "Placeholder 0 start position incorrect");
        static_assert(fmt.placeholder_positions.at(0).second == 15, "Placeholder 0 end position incorrect");
        static_assert(fmt.placeholder_positions.at(1).first == 21, "Placeholder 1 start position incorrect");
        static_assert(fmt.placeholder_positions.at(1).second == 24, "Placeholder 1 end position incorrect");

        static_assert(fmt.fmt.substr(14, 15) == "{", "Substring for placeholder 0 failed");
        static_assert(fmt.fmt.substr(21, 24) == "{%d", "Substring for placeholder 1 failed");

    }
}