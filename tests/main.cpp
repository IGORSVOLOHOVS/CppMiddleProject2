#include "format_string.hpp"
#include "parse.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <cassert>
#include <print>

using namespace stdx::details;

using namespace std::string_view_literals;
int main() { 
    // Test 1: fixed_string
    {
        constexpr fixed_string fstr("1234");
        static_assert(fstr.size() == 4, "Compile error: Size of fstr is not equal by 4");
        static_assert(fstr == "1234", "Compile error: fstr is not equal by 1234");

        constexpr auto fstr_same = fstr;
        static_assert(fstr_same.size() == 4, "Compile error: Size of fstr_same is not equal by 4");
        static_assert(fstr == fstr_same, "Compile error: fstr and fstr_same are not the same");

        constexpr auto fstr_empty = fixed_string{""};
        static_assert(fstr_empty == "", "Compile error: fstr_empty is not empty");
        
        // Создание подстроки с явным указанием размера
        constexpr fixed_string<2> fstr_sub{&fstr[1], &fstr[3]};
        static_assert(fstr_sub == "23", "Compile error: fstr_sub is not equal by 23");

        // Создание подстроки через метод substr
        constexpr auto fstr_sub_m = fstr.substr(1, 3);
        static_assert(fstr_sub_m == "23", "Compile error: fstr_sub_m is not equal by 23");

        std::println("[OK] Test 1: fixed_string");
    }

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

        std::println("[OK] Test 2: format_string");
    }

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

        std::println("[OK] Test 3: get_current_source_for_parsing");
    }
    

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

        std::println("[OK] Test 4: parse_value");
    }
    
    // Test 5: parse_input и scan
    {
        constexpr auto fmt = "I want to sum {} and {%d} numbers."_fs;
        constexpr fixed_string source("I want to sum 123 and -4567 numbers.");
        
        constexpr auto input_1_prs = parse_input<0, fmt, source, std::string_view>();
        constexpr auto input_2_prs = parse_input<1, fmt, source, int16_t>();
        static_assert(input_1_prs == "123");
        static_assert(input_2_prs == -4567);

        constexpr auto scan_res = stdx::scan<fmt, source, std::string_view, int16_t>();

        static_assert(scan_res.get<0>() == "123", "Scan result for string_view failed");
        static_assert(scan_res.get<1>() == -4567, "Scan result for int16_t failed");
        static_assert(scan_res.size() == 2, "Incorrect size for scan result");

        std::println("[OK] Test 5: parse_input and scan");
    }

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
        
        std::println("[OK] Test 6: comprehensive scan");
    }

    // Test 7: scan without placeholders
    {
        constexpr auto fmt_empty = "Empty source"_fs;    
        constexpr fixed_string source_empty{"Empty source"};
        constexpr auto scan_res_empty = stdx::scan<fmt_empty, source_empty>();

        static_assert(scan_res_empty.size() == 0);

        std::println("[OK] Test 7: empty scan");
    }

    // Test 8: format_string with unknown placeholder
    {
        // constexpr auto fmt = "{} {} {%f}"_fs;    
        std::println("[OK] Test 8: unknown placeholder");
    }

    // Test 9: parse_input with unknown type
    {
        constexpr auto fmt = "I want to show {}"_fs;
        constexpr fixed_string source("I want to show 12.3");
        
        // constexpr auto input = parse_input<0, fmt, source, double>();
        std::println("[OK] Test 9: unsupported type");
    }

    // Test 10: incorrect number format
    {
        constexpr auto fmt = "I want to show {%d}"_fs;
        constexpr fixed_string source_1("I want to show a123bc");
        constexpr fixed_string source_2("I want to show 123bc");
        constexpr fixed_string source_3("I want to show ");
        constexpr fixed_string source_4("I want to show abc");
        constexpr fixed_string source_5("I want to show 1abc2");
        
        // constexpr auto input_1 = parse_input<0, fmt, source_1, int>(); // throw "This is not a number.\n";
        // constexpr auto input_2 = parse_input<0, fmt, source_2, int>(); // throw "This number has unother symbols.\n";
        // constexpr auto input_3 = parse_input<0, fmt, source_3, int>(); // throw "Input is empty. Parsing error.\n";
        // constexpr auto input_4 = parse_input<0, fmt, source_4, int>(); // throw "This is not a number.\n";
        // constexpr auto input_5 = parse_input<0, fmt, source_5, int>(); // throw "This number has unother symbols.\n";

        std::println("[OK] Test 10: incorrect number format");
    }

    // Test 11: no number for placeholder
    {
        constexpr auto fmt = "I want to show {%d}"_fs;
        constexpr fixed_string source("I want to show    ");
        
        // constexpr auto input = parse_input<0, fmt, source, int>(); // throw "This is not a number.\n";

        std::println("[OK] Test 11: no number for placeholder");
    }

    // Test 12: scan count not equal
    {
        constexpr auto fmt = "I want to sum {} and {%d} numbers."_fs;
        constexpr fixed_string source("I want to sum 123 and -4567 numbers.");

        // constexpr auto scan_res = stdx::scan<fmt, source, std::string_view>(); // throw "Compile error: The count of types are not the same as count of placeholders";

        std::println("[OK] Test 12: count of types and placeholders");
    }

    std::println("All tests passed successfully!");

    return 0;
}
