#include "format_string.hpp"
#include "parse.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <cassert>
#include <print>

using namespace stdx::details;

using namespace std::string_view_literals;
int main() { 
    // Test 4: parse_value 
    {
        constexpr auto i8_min  = parse_value<int8_t,  fixed_string("-128")>();
        constexpr auto i8_max  = parse_value<int8_t,  fixed_string("127")>();
        constexpr auto i16_min = parse_value<int16_t, fixed_string("-32768")>();
        constexpr auto i16_max = parse_value<int16_t, fixed_string("32767")>();
        constexpr auto i32_min = parse_value<int32_t, fixed_string("-2147483648")>();
        constexpr auto i32_max = parse_value<int32_t, fixed_string("2147483647")>();
        constexpr auto i64_min = parse_value<int64_t, fixed_string("-9223372036854775808")>();
        constexpr auto i64_max = parse_value<int64_t, fixed_string("9223372036854775807")>();

        static_assert(i8_min  == std::numeric_limits<int8_t>::min());
        static_assert(i8_max  == std::numeric_limits<int8_t>::max());
        static_assert(i16_min == std::numeric_limits<int16_t>::min());
        static_assert(i16_max == std::numeric_limits<int16_t>::max());
        static_assert(i32_min == std::numeric_limits<int32_t>::min());
        static_assert(i32_max == std::numeric_limits<int32_t>::max());
        static_assert(i64_min == std::numeric_limits<int64_t>::min());
        static_assert(i64_max == std::numeric_limits<int64_t>::max());

        constexpr auto u8_max  = parse_value<uint8_t,  fixed_string("255")>();
        constexpr auto u16_max = parse_value<uint16_t, fixed_string("65535")>();
        constexpr auto u32_max = parse_value<uint32_t, fixed_string("4294967295")>();
        constexpr auto u64_max = parse_value<uint64_t, fixed_string("18446744073709551615")>();

        static_assert(u8_max  == std::numeric_limits<uint8_t>::max());
        static_assert(u16_max == std::numeric_limits<uint16_t>::max());
        static_assert(u32_max == std::numeric_limits<uint32_t>::max());
        static_assert(u64_max == std::numeric_limits<uint64_t>::max());

        constexpr auto str_view_value = parse_value<std::string_view, fixed_string("word")>();
        static_assert(str_view_value == "word");

        constexpr auto const_i32_val = parse_value<const int32_t, fixed_string("-32")>();
        static_assert(const_i32_val == -32);

        volatile int16_t volatile_i16_val = parse_value<volatile int16_t, fixed_string("-16000")>();
        assert(volatile_i16_val == -16000);

    }
}