#include <gtest/gtest.h>
#include <print>

#include "parse.hpp"

TEST(ParseTest, LogicOfParseSources) {
    constexpr auto input = "I want to sum 42 and 3.14 numbers.";
    constexpr auto format = "I want to sum {} and {%f} numbers."; 
    auto result = stdx::details::parse_sources(input, format); // 1 - format, 2 - inputs

    ASSERT_EQ(result->first.at(0), "");
    ASSERT_EQ(result->first.at(1), "%f");

    ASSERT_EQ(result->second.at(0), "42");
    ASSERT_EQ(result->second.at(1), "3.14");
}


TEST(ParseTest, ParseValueOfFormat) {
    constexpr auto input = "I want to sum 42 and 3.14 numbers.";
    constexpr auto format = "I want to sum {} and {%f} numbers."; 
    auto result = stdx::details::parse_sources(input, format); // 1 - format, 2 - inputs

    auto input1 = result->second.at(0); // 42
    auto fmt1 = result->first.at(0); // {}

    auto input2 = result->second.at(1); // 3.14
    auto fmt2 = result->first.at(1); // {%f}

    auto result1 = stdx::details::parse_value_with_format<std::string>(input1, fmt1);
    auto result2 = stdx::details::parse_value_with_format<double>(input2, fmt2);

    ASSERT_EQ(result1.value(), "42");
    ASSERT_NEAR(result2.value(), 3.14, 1e-3);
}

