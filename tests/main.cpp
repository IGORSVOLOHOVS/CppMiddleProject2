#include "format_string.hpp"
#include "scan.hpp"
#include "types.hpp"

using namespace stdx::details;

int main() { 
    // Test 1: stdx::details::fixed_string<3>{"123"}
    static_assert(std::char_traits<char>::compare(stdx::details::fixed_string<3>{"123"}.c_str(), "123", 3) == 0);

    // Test 2: parse_error
    static_assert(std::char_traits<char>::compare(stdx::details::parse_error{"123"}.c_str(), "123", 3) == 0);
    
    // Test 3: stdx::details::fixed_string<3>{"{", "}"}
    constexpr const char* input_arr = {"xxx {123} xxx"};
    constexpr const char* from = &input_arr[5];
    constexpr const char* to = &input_arr[8];
    static_assert(std::char_traits<char>::compare(stdx::details::fixed_string<3>{from, to}.c_str(), "123", 3) == 0);

    // Test 4: stdx::details::format_string<"I want to sum {} and {%f} numbers.">
    constexpr const char* format_arr = {"I want to sum {} and {%f} numbers."};

    constexpr const char* from_1 = &format_arr[14];
    constexpr const char* to_1 = &format_arr[15];

    constexpr const char* from_2 = &format_arr[21];
    constexpr const char* to_2 = &format_arr[24];

    // Test 5: _xs
    constexpr auto simple_str = "I want to sum 123 and 4.56 numbers.";
    constexpr auto fixed_str = "I want to sum 123 and 4.56 numbers."_xs;
    static_assert(std::char_traits<char>::compare(fixed_str.c_str(), simple_str, fixed_str.size()) == 0);

    // Test 6: _fs and get_placeholder_positions()
    constexpr auto fmt = "I want to sum {} and {%f} numbers."_fs;
    constexpr auto res = fmt.get_placeholder_positions();
    static_assert(res.at(0).first == 14);
    static_assert(res.at(0).second == 15);
    static_assert(res.at(1).first == 21);
    static_assert(res.at(1).second == 24);
    static_assert(res.size() == 2);

    // Test 7: get_current_source_for_parsing
    constexpr auto input_1 = get_current_source_for_parsing<0, fmt, fixed_str>();
    constexpr auto input_2 = get_current_source_for_parsing<1, fmt, fixed_str>();
    static_assert(input_1.first == 14);
    static_assert(input_1.second == 17);
    static_assert(input_2.first == 22);
    static_assert(input_2.second == 26);

    // Test 8: parse_input
    constexpr auto input_1_prs = parse_input<0, fmt, fixed_str, int>();
    constexpr auto input_2_prs = parse_input<1, fmt, fixed_str, double>();

    // static_assert(stdx::scan<stdx::details::format_string{}, stdx::details::fixed_string{}, int>().i == 42); 
}