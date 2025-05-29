#include "scan.hpp"
#include "types.hpp"

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
    
    // static_assert(stdx::scan<stdx::details::format_string{}, stdx::details::fixed_string{}, int>().i == 42); 
}