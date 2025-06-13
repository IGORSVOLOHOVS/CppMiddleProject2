#include "format_string.hpp"
#include "parse.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <cassert>
#include <print>

using namespace stdx::details;

using namespace std::string_view_literals;
int main() { 
    // Test 6: scan (all parametrs)
    {
        constexpr auto fmt_all_types = "Here are numbers: {%d} {%d} {%d} {%d} and {%u} {%u} {%u} {%u} and words {} and {%s}"_fs;    
        constexpr fixed_string source_all_types{"Here are numbers: -128 -32768 -2147483647 9223372036854775807 and 129 32769 2147483649 18446744073709551615 and words abc_def and igors"};
        constexpr auto scan_res_all_types = stdx::scan<fmt_all_types, source_all_types, int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, std::string_view, std::string_view>();

        static_assert(scan_res_all_types.get<0>() == -128);
        static_assert(scan_res_all_types.get<1>() == -32768);
        static_assert(scan_res_all_types.get<2>() == -2147483647);
        static_assert(scan_res_all_types.get<3>() == 9223372036854775807LL);
        static_assert(scan_res_all_types.get<4>() == 129);
        static_assert(scan_res_all_types.get<5>() == 32769);
        static_assert(scan_res_all_types.get<6>() == 2147483649);
        static_assert(scan_res_all_types.get<7>() == 18446744073709551615ULL);
        static_assert(scan_res_all_types.get<8>() == "abc_def");
        static_assert(scan_res_all_types.get<9>() == "igors");
        static_assert(scan_res_all_types.size() == 10);
        
    }
}