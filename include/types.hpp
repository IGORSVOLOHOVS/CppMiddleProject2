#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <tuple>

namespace stdx::details {

template<std::size_t N>
struct fixed_string {
    constexpr fixed_string(const char (&arr)[N]){
        std::copy_n(arr, N, data.data());
    }

    template<std::size_t S>
    constexpr fixed_string(const char (&arr)[S]){
        static_assert(S == 0 || S <= N, "Error: the size of object is less then the size of copy object or is equal by zero");
        std::copy_n(arr, S, data.data());
    }

    constexpr fixed_string(const char *from, const char* to) {
        if (from > to){
            throw "Invalid range"; 
        } 

        const size_t count_to_copy = std::min(static_cast<size_t>(to - from), N);
        std::copy_n(from, count_to_copy, data.data());
    }

    constexpr size_t size() const {
        const auto it = std::find(data.begin(), data.end(), '\0');  
        return std::distance(data.begin(), it);
    }

    constexpr std::string_view view() const {
        return {data.data(), size()};
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

    template<size_t M>  
    constexpr bool operator==(const char (&arr)[M]) const {  
        if (this->size() != (M - 1)) {
            return false;
        }
        return std::equal(data.begin(), data.begin() + this->size(), arr);  
    }  

    std::array<char, N> data{};
};

// Направляющий дедукционный список  
template<std::size_t N>  
fixed_string(const char (&)[N]) -> fixed_string<N>;  

constexpr const size_t MAX_PARSE_ERROR_MSG_SIZE = 43;
using parse_error = fixed_string<MAX_PARSE_ERROR_MSG_SIZE>; 

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