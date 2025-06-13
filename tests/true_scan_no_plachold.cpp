#include "format_string.hpp"
#include "parse.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <cassert>
#include <print>

using namespace stdx::details;

using namespace std::string_view_literals;
int main() { 
    // Test 7: scan without placeholders
    {
        constexpr auto fmt_empty = "Empty source"_fs;    
        constexpr fixed_string source_empty{"Empty source"};
        constexpr auto scan_res_empty = stdx::scan<fmt_empty, source_empty>();

        static_assert(scan_res_empty.size() == 0);

    }
}