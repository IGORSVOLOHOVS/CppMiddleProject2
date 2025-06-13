#pragma once

#include <tuple>

#include "parse.hpp"
#include "format_string.hpp"
#include "types.hpp"

namespace stdx {


    template <details::format_string fmt, details::fixed_string source, typename... Ts, std::size_t... Is>
    consteval auto scan_impl(std::index_sequence<Is...>) {
        if(fmt.number_placeholders != sizeof...(Ts)){
            throw "Compile error: The count of types are not the same as count of placeholders";
        }

        constexpr auto res = std::make_tuple(details::parse_input<Is, fmt, source, Ts>()...);
        return details::scan_result<Ts...>{res};
    }

    // Главная функция
    template <details::format_string fmt, details::fixed_string source, typename... Ts>
    consteval details::scan_result<Ts...> scan() { 
        return scan_impl<fmt, source, Ts...>(std::index_sequence_for<Ts...>{});
    }

} // namespace stdx