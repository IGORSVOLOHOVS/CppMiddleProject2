#include "format_string.hpp"
#include "parse.hpp"
#include "scan.hpp"
#include "types.hpp"

using namespace stdx::details;

using namespace std::string_view_literals;
int main() { 
    // Test 1: fixed_string
    constexpr const fixed_string str("1234");
    constexpr auto str_same = str;
    constexpr const auto str2 = fixed_string{""};
    constexpr const auto str3 = fixed_string<6>{"123"};
    static_assert(str == "1234");
    static_assert(str3 == "123");
    static_assert(str3.size() == 3);
    static_assert(str == str_same);
    
    constexpr auto sub = fixed_string(&str[1], &str[3]);
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
    constexpr auto int8_t_value = parse_value<int8_t, fixed_string("12")>();

    // Test 4: parse_input
    // constexpr auto input_1_prs = parse_input<0, fmt, source, std::string_view>();
    // constexpr auto input_2_prs = parse_input<1, fmt, source, int16_t>();

}
// int main() { 
//     // Test 1: stdx::details::fixed_string<3>{"123"}
//     static_assert(std::char_traits<char>::compare(stdx::details::fixed_string<3>{"123"}.c_str(), "123", 3) == 0);

//     // Test 2: parse_error
//     static_assert(std::char_traits<char>::compare(stdx::details::parse_error{"123"}.c_str(), "123", 3) == 0);
    
//     // Test 3: stdx::details::fixed_string<3>{"{", "}"}
//     constexpr const char* input_arr = {"xxx {123} xxx"};
//     constexpr const char* from = &input_arr[5];
//     constexpr const char* to = &input_arr[8];
//     static_assert(std::char_traits<char>::compare(stdx::details::fixed_string<3>{from, to}.c_str(), "123", 3) == 0);

//     // Test 4: stdx::details::format_string<"I want to sum {} and {%f} numbers.">
//     constexpr const char* format_arr = {"I want to sum {} and {%u} numbers."};

//     constexpr const char* from_1 = &format_arr[14];
//     constexpr const char* to_1 = &format_arr[15];

//     constexpr const char* from_2 = &format_arr[21];
//     constexpr const char* to_2 = &format_arr[24];

//     // Test 5: _xs
//     constexpr auto simple_str = "I want to sum 123 and 4567 numbers.";
//     constexpr auto fixed_str = "I want to sum 123 and 4567 numbers."_xs;
//     static_assert(std::char_traits<char>::compare(fixed_str.c_str(), simple_str, fixed_str.size()) == 0);

//     // Test 6: _fs and get_placeholder_positions()
//     constexpr auto fmt = "I want to sum {} and {%u} numbers."_fs;
//     constexpr auto res = fmt.get_placeholder_positions();
//     static_assert(res.at(0).first == 14);
//     static_assert(res.at(0).second == 15);
//     static_assert(res.at(1).first == 21);
//     static_assert(res.at(1).second == 24);
//     static_assert(res.size() == 2);

//     constexpr auto fmt_1 = stdx::details::fixed_string<0>{&fmt.fmt.data[res.at(0).first + 1], &fmt.fmt.data[res.at(0).second]};
//     constexpr auto fmt_2 = stdx::details::fixed_string<2>{&fmt.fmt.data[res.at(1).first + 1], &fmt.fmt.data[res.at(1).second]};
//     static_assert(std::char_traits<char>::compare(fmt_1.c_str(), "", 0) == 0);
//     static_assert(std::char_traits<char>::compare(fmt_2.c_str(), "%u", 2) == 0);

//     // Test 7: get_current_source_for_parsing
//     constexpr auto input_1 = get_current_source_for_parsing<0, fmt, fixed_str>();
//     constexpr auto input_2 = get_current_source_for_parsing<1, fmt, fixed_str>();
//     static_assert(input_1.first == 14);
//     static_assert(input_1.second == 17);
//     static_assert(input_2.first == 22);
//     static_assert(input_2.second == 26);

//     constexpr auto src_1 = stdx::details::fixed_string<3>{&fixed_str.data[input_1.first], &fixed_str.data[input_1.second]};
//     constexpr auto src_2 = stdx::details::fixed_string<4>{&fixed_str.data[input_2.first], &fixed_str.data[input_2.second]};
//     static_assert(std::char_traits<char>::compare(src_1.c_str(), "123", 3) == 0);
//     static_assert(std::char_traits<char>::compare(src_2.c_str(), "4567", 4) == 0);

//     // Test 8: parse_input
//     constexpr auto fmt_input = "I want to sum {} and {%u} numbers."_fs;
//     constexpr auto fixed_input = "I want to sum 123 and 4567 numbers."_xs;

//     constexpr auto input_1_prs = parse_input<0, fmt_input, fixed_input, std::string_view>();
//     constexpr auto input_2_prs = parse_input<1, fmt_input, fixed_input, uint16_t>();

//     static_assert(input_1_prs == "123");
//     static_assert(input_2_prs == 4567);

//     // Test 9: scan
//     constexpr auto fmt_scan = "I want to sum {} and {%u} numbers."_fs;
//     constexpr auto fixed_scan = "I want to sum 123 and 4567 numbers."_xs;
//     constexpr auto scan_res = stdx::scan<fmt_scan, fixed_scan, std::string_view, uint16_t>();

//     constexpr auto vv = scan_res.values_;
    
//     static_assert(std::get<std::string_view>(vv) == "123");
//     static_assert(std::get<uint16_t>(vv) == 4567);
//     // Поддержано сканирование в типы int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t и 
//     // std::string_view, а также cv-квалифицированные версии этих типов. Передача любых других типов должна 
//     // генерировать ошибку компиляции с человекочитаемым сообщением.
//     // Test 10: all types
//     constexpr auto fmt_scan_2 = "Here are numbers: {%d} {%d} {%d} {%d} and {%u} {%u} {%u} {%u} and words {} and {%s}"_fs;
//     constexpr auto fixed_scan_2 = "Here are numbers: -128 -32768 -2147483648 3147483648 and 129 32769 2147483649 3147483648 and words abc_def and igors"_xs;
//     constexpr auto scan_res_2 = stdx::scan<fmt_scan_2, fixed_scan_2, int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, std::string_view, const char*>();

//     constexpr auto vv2 = scan_res.values_;
    
//     static_assert(std::get<int8_t>(vv2) == -128);
//     static_assert(std::get<int16_t>(vv2) == -32768);
//     static_assert(std::get<int32_t>(vv2) == -2147483648);
//     static_assert(std::get<int64_t>(vv2) == 3147483648);
//     static_assert(std::get<uint8_t>(vv2) == 129);
//     static_assert(std::get<uint16_t>(vv2) == 32769);
//     static_assert(std::get<uint32_t>(vv2) == 2147483649);
//     static_assert(std::get<uint64_t>(vv2) == 3147483648);
//     static_assert(std::get<std::string_view>(vv2) == "abc_def");
//     static_assert(std::get<std::string_view>(vv2) == "igors");
// }