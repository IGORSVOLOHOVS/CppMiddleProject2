#pragma once

#include <concepts>
#include <cstring>
#include <expected>

#include "types.hpp"

namespace stdx::details {

template<fixed_string str>
class format_string {
    static consteval std::expected<size_t, parse_error> get_number_placeholders() {
        constexpr size_t N = str.size();
        if (!N){
            return std::unexpected(parse_error{"The size of fixed array is equal by zero"});
        }

        size_t placeholder_count = 0;
        size_t pos = 0;
        const size_t size = N;

        while (pos < size) {
            // Пропускаем все символы до '{'
            if (str.data[pos] != '{') {
                ++pos;
                continue;
            }

            // Проверяем незакрытый плейсхолдер
            if (pos + 1 >= size) {
                return std::unexpected(parse_error{"Unclosed last placeholder"});
            }

            // Начало плейсхолдера
            ++placeholder_count;
            ++pos;

            // Проверка спецификатора формата
            if (str.data[pos] == '%') {
                ++pos;
                if (pos >= size) {
                    return std::unexpected(parse_error{"Unclosed last placeholder"});
                }

                // Проверяем допустимые спецификаторы
                const char spec = str.data[pos];
                constexpr std::array valid_specs = {'d', 'u', 's'};
                bool valid = false;

                for (const char s : valid_specs) {
                    if (spec == s) {
                        valid = true;
                        break;
                    }
                }

                if (!valid) {
                    return std::unexpected(parse_error{"Invalid specifier."});
                }
                ++pos;
            }

            // Проверяем закрывающую скобку
            if (pos >= size || str.data[pos] != '}') {
                return std::unexpected(parse_error{"\'}\' hasn't been found in appropriate place"});
            }

            ++pos;
        }

        return placeholder_count;
    }

    static consteval auto get_placeholder_positions() {
        std::array<std::pair<size_t, size_t>, number_placeholders> placeholder_positions;

        size_t from_pos = 0;
        size_t to_pos = 0;
        size_t end_pos = str.size();
        size_t iter = 0;

        constexpr auto data_view = str.view();
        while(to_pos < end_pos){
            from_pos = data_view.find_first_of('{', to_pos);
            if(from_pos == std::string_view::npos){
                break;
            }

            to_pos = data_view.find_first_of('}', from_pos);
            if(to_pos == std::string_view::npos){
                throw "\'}\' hasn't been found in appropriate place";
            }

            placeholder_positions[iter++] = {from_pos, to_pos};
        }
        return placeholder_positions;
    }

public:
    static constexpr auto fmt = str;
    static constexpr auto number_placeholders = get_number_placeholders().value();
    static constexpr auto placeholder_positions = get_placeholder_positions();
};

template<fixed_string str>
constexpr auto operator""_fs() {
    return format_string<str>{};
}

} // namespace stdx::details