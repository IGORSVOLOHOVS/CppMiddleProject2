#include "format_string.hpp"
#include "parse.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <cassert>

using namespace stdx::details;

using namespace std::string_view_literals;
int main() { 
    // Test 1: fixed_string
    constexpr const fixed_string str("1234");
    constexpr auto str_same = str;
    constexpr const auto str2 = fixed_string{""};
    static_assert(str == "1234");
    static_assert(str == str_same);
    
    constexpr auto sub = fixed_string{&str[1], &str[3]};
    static_assert(sub.size() == 2);
    static_assert(sub == "23");

    constexpr auto sub2 = str.substr(1, 3);
    static_assert(sub2.size() == 2);
    static_assert(sub2 == "23");

    // Test 2: format_string
    constexpr auto fmt = "I want to sum {} and {%d} numbers."_fs;

    static_assert(fmt.fmt == "I want to sum {} and {%d} numbers.");
    static_assert(fmt.number_placeholders == 2);
    static_assert(fmt.placeholder_positions.at(0).first == 14);
    static_assert(fmt.placeholder_positions.at(0).second == 15);
    static_assert(fmt.placeholder_positions.at(1).first == 21);
    static_assert(fmt.placeholder_positions.at(1).second == 24);

    static_assert(fmt.fmt .substr(14, 15) == "{");
    static_assert(fmt.fmt .substr(21, 24) == "{%d");

    // Test 3:  get_current_source_for_parsing
    constexpr const fixed_string source("I want to sum 123 and -4567 numbers.");

    constexpr auto source_1 = get_current_source_for_parsing<0, fmt, source>();
    constexpr auto source_2 = get_current_source_for_parsing<1, fmt, source>();

    static_assert(source_1.first == 14);
    static_assert(source_1.second == 17);
    static_assert(source_2.first == 22);
    static_assert(source_2.second == 27);

    static_assert(source.substr(source_1.first, source_1.second) == "123");
    static_assert(source.substr(source_2.first, source_2.second) == "-4567");

    // Test 4: parse_value
    constexpr int8_t int8_t_value = parse_value<int8_t, fixed_string("-8")>();
    constexpr int16_t int16_t_value = parse_value<int16_t, fixed_string("-16")>();
    constexpr int32_t int32_t_value = parse_value<int32_t, fixed_string("-32")>();
    constexpr int64_t int64_t_value = parse_value<int64_t, fixed_string("-64")>();
    constexpr uint8_t uint8_t_value = parse_value<uint8_t, fixed_string("8")>();
    constexpr uint16_t uint16_t_value = parse_value<uint16_t, fixed_string("16")>();
    constexpr uint32_t uint32_t_value = parse_value<uint32_t, fixed_string("32")>();
    constexpr uint64_t uint64_t_value = parse_value<uint64_t, fixed_string("64")>();
    constexpr std::string_view str_view_value = parse_value<std::string_view, fixed_string("word")>();

    constexpr const int8_t const_int8_t_value = parse_value<const int8_t, fixed_string("-8")>();
    constexpr const int16_t const_int16_t_value = parse_value<const int16_t, fixed_string("-16")>();
    constexpr const int32_t const_int32_t_value = parse_value<const int32_t, fixed_string("-32")>();
    constexpr const int64_t const_int64_t_value = parse_value<const int64_t, fixed_string("-64")>();
    constexpr const uint8_t const_uint8_t_value = parse_value<const uint8_t, fixed_string("8")>();
    constexpr const uint16_t const_uint16_t_value = parse_value<const uint16_t, fixed_string("16")>();
    constexpr const uint32_t const_uint32_t_value = parse_value<const uint32_t, fixed_string("32")>();
    constexpr const uint64_t const_uint64_t_value = parse_value<const uint64_t, fixed_string("64")>();
    constexpr const std::string_view const_str_view_value = parse_value<const std::string_view, fixed_string("word")>();

    constexpr volatile int8_t volatile_int8_t_value = parse_value<volatile int8_t, fixed_string("-8")>();
    constexpr volatile int16_t volatile_int16_t_value = parse_value<volatile int16_t, fixed_string("-16")>();
    constexpr volatile int32_t volatile_int32_t_value = parse_value<volatile int32_t, fixed_string("-32")>();
    constexpr volatile int64_t volatile_int64_t_value = parse_value<volatile int64_t, fixed_string("-64")>();
    constexpr volatile uint8_t volatile_uint8_t_value = parse_value<volatile uint8_t, fixed_string("8")>();
    constexpr volatile uint16_t volatile_uint16_t_value = parse_value<volatile uint16_t, fixed_string("16")>();
    constexpr volatile uint32_t volatile_uint32_t_value = parse_value<volatile uint32_t, fixed_string("32")>();
    constexpr volatile uint64_t volatile_uint64_t_value = parse_value<volatile uint64_t, fixed_string("64")>();
    constexpr volatile std::string_view volatile_str_view_value = parse_value<std::string_view, fixed_string("word")>();   

    static_assert(int8_t_value == -8);
    static_assert(int16_t_value == -16);
    static_assert(int32_t_value == -32);
    static_assert(int64_t_value == -64);
    static_assert(uint8_t_value == 8);
    static_assert(uint16_t_value == 16);
    static_assert(uint32_t_value == 32);
    static_assert(uint64_t_value == 64);
    static_assert(str_view_value == "word");

    static_assert(const_int8_t_value == -8);
    static_assert(const_int16_t_value == -16);
    static_assert(const_int32_t_value == -32);
    static_assert(const_int64_t_value == -64);
    static_assert(const_uint8_t_value == 8);
    static_assert(const_uint16_t_value == 16);
    static_assert(const_uint32_t_value == 32);
    static_assert(const_uint64_t_value == 64);
    static_assert(const_str_view_value == "word");

    assert(volatile_int8_t_value == -8);
    assert(volatile_int16_t_value == -16);
    assert(volatile_int32_t_value == -32);
    assert(volatile_int64_t_value == -64);
    assert(volatile_uint8_t_value == 8);
    assert(volatile_uint16_t_value == 16);
    assert(volatile_uint32_t_value == 32);
    assert(volatile_uint64_t_value == 64);
    // assert(std::string_view{volatile_str_view_value} == "word");

    // Test 4: parse_input
    constexpr auto input_1_prs = parse_input<0, fmt, source, std::string_view>();
    constexpr auto input_2_prs = parse_input<1, fmt, source, int16_t>();

    // Test 5: scan
    constexpr auto scan_res = stdx::scan<fmt, source, std::string_view, int16_t>();

    static_assert(scan_res.get<0>() == "123");
    static_assert(scan_res.get<1>() == -4567);
    static_assert(scan_res.size() == 2);

    constexpr auto fmt_all_types = "Here are numbers: {%d} {%d} {%d} {%d} and {%u} {%u} {%u} {%u} and words {} and {%s}"_fs;    
    constexpr auto source_all_types = fixed_string{"Here are numbers: -128 -32768 -2147483647 3147483648 and 129 32769 2147483649 3147483648 and words abc_def and igors"};
    constexpr auto scan_res_all_types = stdx::scan<fmt_all_types, source_all_types, int8_t, int16_t, int32_t, const int64_t, uint8_t, uint16_t, const uint32_t, uint64_t, std::string_view, std::string_view>();

    static_assert(scan_res_all_types.get<0>() == -128);
    static_assert(scan_res_all_types.get<1>() == -32768);
    static_assert(scan_res_all_types.get<2>() == -2147483647);
    static_assert(scan_res_all_types.get<3>() == 3147483648);
    static_assert(scan_res_all_types.get<4>() == 129);
    static_assert(scan_res_all_types.get<5>() == 32769);
    static_assert(scan_res_all_types.get<6>() == 2147483649);
    static_assert(scan_res_all_types.get<7>() == 3147483648);
    static_assert(scan_res_all_types.get<8>() == "abc_def");
    static_assert(scan_res_all_types.get<9>() == "igors");
    static_assert(scan_res_all_types.size() == 10);

    constexpr auto fmt_empty = "Empty source"_fs;    
    constexpr auto source_empty  = fixed_string{"Empty source"};
    constexpr auto scan_res_empty = stdx::scan<fmt_empty, source_empty>();

    static_assert(scan_res_empty.size() == 0);
}
