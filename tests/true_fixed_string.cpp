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

    }
}