#pragma once

#include <concepts>
#include <cstring>
#include <expected>

#include "types.hpp"

namespace stdx::details {

// Шаблонный класс для хранения форматирующей строчки и ее особенностей
// ваш код здесь
template<fixed_string str>
class format_string {
public:
    static consteval std::expected<size_t, parse_error> get_number_placeholders() {
        std::expected<size_t, parse_error> number_placeholders;

        constexpr size_t N = str.size();
        if (!N)
            number_placeholders = 0;
        size_t placeholder_count = 0;
        size_t pos = 0;
        const size_t size = N - 1; // -1 для игнорирования нуль-терминатора

        while (pos < size) {
            // Пропускаем все символы до '{'
            if (str.data[pos] != '{') {
                ++pos;
                continue;
            }

            // Проверяем незакрытый плейсхолдер
            if (pos + 1 >= size) {
                number_placeholders = std::unexpected(parse_error{"Unclosed last placeholder"});
            }

            // Начало плейсхолдера
            ++placeholder_count;
            ++pos;

            // Проверка спецификатора формата
            if (str.data[pos] == '%') {
                ++pos;
                if (pos >= size) {
                    number_placeholders = std::unexpected(parse_error{"Unclosed last placeholder"});
                }

                // Проверяем допустимые спецификаторы
                const char spec = str.data[pos];
                constexpr char valid_specs[] = {'d', 'u', 'f', 's'};
                bool valid = false;

                for (const char s : valid_specs) {
                    if (spec == s) {
                        valid = true;
                        break;
                    }
                }

                if (!valid) {
                    number_placeholders = std::unexpected(parse_error{"Invalid specifier."});
                }
                ++pos;
            }

            // Проверяем закрывающую скобку
            if (pos >= size || str.data[pos] != '}') {
                number_placeholders = std::unexpected(parse_error{"\'}\' hasn't been found in appropriate place"});
            }

            ++pos;
        }


        static_assert(!std::same_as<decltype(number_placeholders), std::unexpected<parse_error>>);

        return placeholder_count;
    }

    static consteval decltype(auto) get_placeholder_positions() {
        std::array<std::pair<size_t, size_t>, number_placeholders> placeholder_positions;

        size_t curr_placeholder = 0;
        size_t pos = 0;
        constexpr size_t end_pos = str.data.size();

        size_t from_pos = 0;
        size_t to_pos = 0;
        while (pos < end_pos) {
            // Пропускаем все символы до '{'
            while (pos < end_pos && str.data[pos] != '{') {
                ++pos;
            }
            from_pos = pos;

            // Пропускаем все символы до '{'
            while (pos < end_pos && str.data[pos] != '}') {
                ++pos;
            }
            to_pos = pos;

            if(pos < end_pos)
                placeholder_positions[curr_placeholder++] = {from_pos, to_pos};
        }
        return placeholder_positions;
    }

    static constexpr fixed_string<str.data.size()> fmt = {str.data.data()};
    static constexpr size_t number_placeholders = get_number_placeholders().value();
    static constexpr std::array<std::pair<size_t, size_t>, number_placeholders> placeholder_positions = get_placeholder_positions();
};

// Пользовательский литерал
/*
ваш код здесь
ваш код здесь operator"" _fs()  сигнатуру также поменяйте
{
ваш код здесь
}
*/
template<fixed_ints arr>
constexpr auto operator""_fs() {
    // Возвращаем пустой объект, у которого тип содержит всю информацию
    return format_string<fixed_string<arr.data.size()>{arr.data.data()}>{};
}
// Функция для получения количества плейсхолдеров и проверки корректности формирующей строки
// Функция закомментирована, так как еще не реализованы классы, которые она использует
/*
// Сделайте эту свободную функцию методом класса format_string
template<fixed_string str>
consteval std::expected<size_t, parse_error> get_number_placeholders() {
    constexpr size_t N = str.size();
    if (!N)
        return 0;
    size_t placeholder_count = 0;
    size_t pos = 0;
    const size_t size = N - 1; // -1 для игнорирования нуль-терминатора

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
            constexpr char valid_specs[] = {'d', 'u', 'f', 's'};
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
*/

// Функция для получения позиций плейсхолдеров

// ваш код здесь
// void get_placeholder_positions() {  // сигнатуру тоже нужно изменить
//     // ваш код здесь
// }

} // namespace stdx::details