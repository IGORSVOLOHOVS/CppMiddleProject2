#include "format_string.hpp"
#include "parse.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <cassert>
#include <print>

using namespace stdx::details;

using namespace std::string_view_literals;
int main() { 
    // Test 8: format_string with unknown placeholder
    {
        constexpr auto fmt = "{} {} {%f}"_fs;    
    }
}