#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstring>
#include <iterator>
namespace stdx::details {

// Шаблонный класс, хранящий C-style строку фиксированной длины

// ваш код здесь
template<std::size_t N>
struct fixed_string {
    constexpr fixed_string(const char* arr){
        for(size_t i = 0; i < N; i++){
            if(arr[i] == '\0')
                break;

            data[i] = arr[i];
        }
    }

    constexpr fixed_string(const char* from, const char* to){
        size_t i = 0;
        for(const char* current = from; current != to; current++){
            if(i < N){
                data[i++] = *current;
            }else{
                break;
            }
        }
    }

    constexpr size_t size() const {
        for(size_t i = 0; i < N; i++){
            if(data[i] == '\0')
                return i + 1;
        }
        return N;
    }

    constexpr const char* c_str() const {
        return data.data();
    }

    // ваш код здесь
    std::array<char, N> data{};
};

template<std::size_t N>
struct fixed_ints{
    std::array<char, N> data;

    template <std::size_t... Is>
    constexpr fixed_ints(const char (&arr)[N], std::integer_sequence<std::size_t, Is...>) : data{arr[Is]...} {}
 
    constexpr fixed_ints(char const(&arr)[N]) : fixed_ints(arr, std::make_integer_sequence<std::size_t, N>())
    {}
};

template<fixed_ints arr>
constexpr auto operator""_xs(){
    return fixed_string<arr.data.size()>{arr.data.data()};
}

// Шаблонный класс, хранящий fixed_string достаточной длины для хранения ошибки парсинга

// ваш код здесь
constexpr const char* MAX_PARSE_ERROR_MSG = "\'}\' hasn't been found in appropriate place"; 
constexpr const size_t MAX_PARSE_ERROR_MSG_SIZE = 42;

struct parse_error : public fixed_string<MAX_PARSE_ERROR_MSG_SIZE>{};

// Шаблонный класс для хранения результатов парсинга

template <typename... Ts>
struct scan_result {
    std::tuple<Ts...> values_;

    constexpr std::tuple<Ts...> values(){
        return values_;
    }
};

} // namespace stdx::details