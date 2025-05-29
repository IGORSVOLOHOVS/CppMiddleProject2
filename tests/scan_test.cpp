#include <gtest/gtest.h>
#include <print>

#include "scan.hpp"
#include "types.hpp"

TEST(ScanTest, Scan2Args_True) {
    constexpr auto input = "I want to sum 42 and 3.14 numbers.";
    constexpr auto format = "I want to sum {} and {%f} numbers."; 

    auto result = stdx::scan<std::string, double>(input, format);

    auto values = result->values();
    
    ASSERT_EQ(std::get<0>(values), "42");
    ASSERT_EQ(std::get<1>(values), 3.14);
}

TEST(ScanTest, Scan3Args_True) {
    constexpr auto input = "I want to sum 42 and 3.14 numbers. It is 45.14";
    constexpr auto format = "I want to sum {%u} and {%f} numbers. It is {%s}"; 

    auto result = stdx::scan<unsigned int, double, std::string>(input, format);

    auto values = result->values();
    
    ASSERT_EQ(std::get<0>(values), 42);
    ASSERT_EQ(std::get<1>(values), 3.14);
    ASSERT_EQ(std::get<2>(values), "45.14");
}


TEST(ScanTest, Scan0Args_Exception) {
    constexpr auto input = "I want to sum 42 and 3.14 numbers. It is 45.14";
    constexpr auto format = "I want to sum {%u} and {%f} numbers. It is {%s}"; 
    
    ASSERT_THROW({
        auto result = stdx::scan<>(input, format);
    }, stdx::details::scan_error);
}
