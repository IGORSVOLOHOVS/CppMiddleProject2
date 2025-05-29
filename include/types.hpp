#pragma once

#include <expected>
#include <string>
#include <tuple>

namespace stdx::details {

// Класс для хранения ошибки неуспешного сканирования

struct scan_error {
    std::string message;
};

// Шаблонный класс для хранения результатов успешного сканирования

template <typename... Ts>
struct scan_result {
    std::tuple<std::expected<Ts, scan_error>...> values_;

    constexpr auto values() const {
        return std::make_tuple(std::get<std::expected<Ts, scan_error>>(values_).value()...);
    }
    // здесь ваш код
};

} // namespace stdx::details
