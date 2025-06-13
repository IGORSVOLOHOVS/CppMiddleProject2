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

template<int I, format_string fmt, fixed_string source>
consteval auto get_current_source_for_parsing() {
    static_assert(I >= 0 && I < fmt.number_placeholders, "Invalid placeholder index");

    constexpr auto fmt_sv = fmt.fmt.view();
    constexpr auto src_sv = source.view();
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
template <std::integral T, fixed_string input>
constexpr T parse_value() {
    std::remove_cv_t<T> result = 0;
    constexpr size_t size = input.size();
    if(size == 0){
        throw "Input is empty. Parsing error.\n";
    }

    auto [ptr, errc] = std::from_chars(&input[0], &input[0] + size, result);
    if (errc == std::errc::invalid_argument){
        throw "This is not a number.\n";
    }else if (errc == std::errc::result_out_of_range) {
        throw "This number is larger than an std::remove_cv_t<T>.\n";
    }else if (ptr != &input[0] + size) {
        throw "This number has unother symbols.\n";
    }else if(errc != std::errc()){
        throw "Parsing error!";
    }
    return result;
}

template<typename T>
concept convertible_to_string_view = std::is_convertible_v<T, std::string_view>;

template<convertible_to_string_view T, fixed_string input>
constexpr T parse_value() {
    return T{input.data.data(), input.size()};
}

template<typename T, fixed_string input>
constexpr T parse_value() {
    throw "Unsupported type";
}

enum class PlaceholderType{
    STRING_VIEW = 0,
    UNSIGNED_INT,
    SIGNED_INT,
};

template<format_string fmt>
constexpr auto get_placeholder_type(){
    constexpr auto fmt_view = fmt.fmt.view();
    if(fmt_view.contains('s')){
        return PlaceholderType::STRING_VIEW;
    }else if(fmt_view.contains('u')){
        return PlaceholderType::UNSIGNED_INT;
    }else if(fmt_view.contains('d')){
        return PlaceholderType::SIGNED_INT;
    }
    return PlaceholderType::STRING_VIEW;
}

// здесь ваш код
template<int I, format_string fmt, fixed_string source, typename T>
constexpr T parse_input() {  // поменяйте сигнатуру
    constexpr auto source_range = get_current_source_for_parsing<I, fmt, source>();
    
    constexpr auto source_from = source_range.first;
    constexpr auto source_to = source_range.second ;
    constexpr auto source_str = source.substr(source_from, source_to);
    
    constexpr auto fmt_from = fmt.placeholder_positions[I].first;
    constexpr auto fmt_to = fmt.placeholder_positions[I].second;
    constexpr auto fmt_str = fmt.fmt.substr(fmt_from, fmt_to);

    switch (get_placeholder_type<fmt>()) {
        case PlaceholderType::STRING_VIEW:{
            return parse_value<T, source_str>();
        }
        case PlaceholderType::UNSIGNED_INT:{
            if constexpr (std::is_unsigned<T>()){
                return parse_value<T, source_str>();
            }
        }
        case PlaceholderType::SIGNED_INT:{
            if constexpr (std::is_signed<T>()){
                return parse_value<T, source_str>();
            }
        }
    }
    throw "Invalid placeholder for the given type, or type mismatch.";
}


} // namespace stdx::details