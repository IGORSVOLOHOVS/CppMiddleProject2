#pragma once

#include <string>

namespace stdx::details {

// Класс для хранения ошибки неуспешного сканирования

struct scan_error {
    std::string message;
};

// Шаблонный класс для хранения результатов успешного сканирования

template <typename... Ts>
struct scan_result {
    std::tuple<Ts...> values_;

    constexpr auto values() const {
        return values_;
    }
    // здесь ваш код
};

} // namespace stdx::details
