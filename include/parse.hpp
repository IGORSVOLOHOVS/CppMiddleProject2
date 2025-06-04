#pragma once

#include <charconv>
#include <concepts>
#include <optional>
#include <system_error>
#include <type_traits>
#include <cstdint>

#include "format_string.hpp"
#include "types.hpp"

namespace stdx::details {

// Шаблонная функция, возвращающая пару позиций в строке с исходными данными, соотвествующих I-ому плейсхолдеру
// Функция закомментирована, так как еще не реализованы классы, которые она использует

template<int I, format_string fmt, fixed_string source>
consteval auto get_current_source_for_parsing() {
    static_assert(I >= 0 && I < fmt.number_placeholders, "Invalid placeholder index");

    constexpr auto to_sv = [](const auto& fs) {
        return std::string_view(fs.data.data(), fs.size() - 1);
    };

    constexpr auto fmt_sv = to_sv(fmt.fmt);
    constexpr auto src_sv = to_sv(source);
    constexpr auto& positions = fmt.placeholder_positions;

    // Получаем границы текущего плейсхолдера в формате
    constexpr auto pos_i = positions[I];
    constexpr size_t fmt_start = pos_i.first, fmt_end = pos_i.second;

    // Находим начало в исходной строке
    constexpr auto src_start = [&]{
        if constexpr (I == 0) {
            return fmt_start;
        } else {
            // Находим конец предыдущего плейсхолдера в исходной строке
            constexpr auto prev_bounds = get_current_source_for_parsing<I-1, fmt, source>();
            const auto prev_end = prev_bounds.second;

            // Получаем разделитель между текущим и предыдущим плейсхолдерами
            constexpr auto prev_fmt_end = positions[I-1].second;
            constexpr auto sep = fmt_sv.substr(prev_fmt_end + 1, fmt_start - (prev_fmt_end + 1));

            // Ищем разделитель после предыдущего значения
            auto pos = src_sv.find(sep, prev_end);
            return pos != std::string_view::npos ? pos + sep.size() : src_sv.size();
        }
    }();

    // Находим конец в исходной строке
    constexpr auto src_end = [&]{
        // Получаем разделитель после текущего плейсхолдера
        if constexpr(fmt_end == (fmt_sv.size() - 1)) {
            return src_sv.size();
        }
        constexpr auto sep = fmt_sv.substr(fmt_end + 1,
            (I < fmt.number_placeholders - 1)
                ? positions[I+1].first - (fmt_end + 1)
                : fmt_sv.size() - (fmt_end + 1));
        // Ищем разделитель после текущего значения
        constexpr auto pos = src_sv.find(sep, src_start);
        return pos != std::string_view::npos ? pos : src_sv.size();
    }();
    return std::pair{src_start, src_end};
}


// Реализуйте семейство функция parse_value

// Шаблонная функция, выполняющая преобразования исходных данных в конкретный тип на основе I-го плейсхолдера
template<std::signed_integral T, fixed_string input>
constexpr T parse_value() {
    const auto s = input.c_str();
    T result = 0;
    size_t pos = 0;
    int sign = 1;

    if (s[0] == '-') {
        sign = -1;
        pos = 1;
    } else if (s[0] == '+') {
        pos = 1;
    }
    
    for (size_t i = pos; i < input.size(); ++i) {
        if (s[i] >= '0' && s[i] <= '9') {
            result = result * 10 + (s[i] - '0');
        }
    }

    return result * sign;
}

template <std::unsigned_integral T, fixed_string input>
constexpr T parse_value() {
    T result = 0;
    size_t pos = 0;
    
    constexpr auto s = input.c_str();
    for (size_t i = pos; i < input.size(); ++i) {
        if (s[i] >= '0' && s[i] <= '9') {
            result = result * 10 + (s[i] - '0');
        }
    }

    return result;
}

template<typename T, fixed_string input>
constexpr T parse_value() {
    return T{input.c_str(), input.size()};
}


// здесь ваш код
template<int I, format_string fmt, fixed_string source, typename T>
constexpr T parse_input() {  // поменяйте сигнатуру
    // здесь ваш код
    constexpr auto value_range = get_current_source_for_parsing<I, fmt, source>();
    constexpr auto format_range = fmt.get_placeholder_positions();

    constexpr auto from = value_range.first;
    constexpr auto to = value_range.second ;
    constexpr auto value_str = fixed_string<to - from>(&source.data[from], &source.data[to]);

    constexpr auto fmt_from = format_range[I].first;
    constexpr auto fmt_to = format_range[I].second;
    constexpr auto fmt_str = fixed_string<fmt_to - fmt_from>(&fmt.fmt.data[fmt_from], &fmt.fmt.data[fmt_to]);

        // 3. Сравниваем плейсхолдер и тип, используя consteval-хелпер
    if constexpr (fmt_str.is_equal("{}") 
                    || fmt_str.is_equal("{%s}") 
                    || (fmt_str.is_equal("{%d}") && std::is_signed_v<T>)
                    || (fmt_str.is_equal("{%u}") && std::is_unsigned_v<T>)
                ) 
    {
        return parse_value<T, value_str>();
    } else {
        static_assert(!std::is_same_v<T, T>, "Invalid placeholder for the given type, or type mismatch.");
    }
    return T{};
    // return parse_value<T, value_str>(); 
}


} // namespace stdx::details