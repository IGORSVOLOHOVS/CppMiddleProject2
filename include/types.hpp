#pragma once

#include <algorithm>
#include <array>
#include <compare>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <tuple>
#include <utility>

namespace stdx::details {

constexpr const std::size_t MAX_SUB_STR = 25;
template<std::size_t N = MAX_SUB_STR>
struct fixed_string {
    constexpr fixed_string(const char (&arr)[N]){
        std::copy_n(arr, N, data.data());
    }

    template<std::size_t S>
    constexpr fixed_string(const char (&arr)[S]){
        static_assert(S <= N, "Error: the size of object is less then the size of copy object");
        std::copy_n(arr, S, data.data());
    }

    constexpr fixed_string(const char* from, const char* to) {
        const size_t count_to_copy = std::min(static_cast<size_t>(to - from), N);
        std::copy_n(from, count_to_copy, data.data());
    }

    constexpr size_t size() const {
        return N - std::count(data.begin(), data.end(), '\0');
    }

    constexpr const char* c_str() const {
        return data.data();
    }

    constexpr const char& operator[](size_t index) const {
        return data.at(index);
    }

    constexpr char& operator[](size_t index) {
        return data[index];
    }

    constexpr auto substr(size_t from, size_t to) const {
        return fixed_string{&operator[](from), &operator[](to)};
    }

    constexpr bool operator==(const fixed_string& fs) const{
        return std::equal(data.begin(), data.end(), fs.data.begin());
    }
    constexpr bool operator==(const char (&arr)[N]) const {
        return std::equal(data.begin(), data.end(), arr);
    }

    std::array<char, N> data{};
};

constexpr const size_t MAX_PARSE_ERROR_MSG_SIZE = 43;
struct parse_error : public fixed_string<MAX_PARSE_ERROR_MSG_SIZE>{};

template <typename... Ts>
struct scan_result {
    std::tuple<Ts...> values_;

    template<size_t I>
    constexpr decltype(auto) get() const{
        return std::get<I>(values_);
    }

    constexpr auto size() const {
        return sizeof...(Ts);
    }
};

} // namespace stdx::details