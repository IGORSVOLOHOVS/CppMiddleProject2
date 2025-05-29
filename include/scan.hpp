#pragma once

#include "parse.hpp"
#include "types.hpp"
#include <tuple>
#include <concepts>

namespace stdx {

// замените болванку функции scan на рабочую версию
template <typename... Ts>
std::expected<details::scan_result<Ts...>, details::scan_error> scan(std::string_view input, std::string_view format) {
    if constexpr (sizeof...(Ts) == 0){
        return details::scan_error{"Nothing to scan!"};
    }
    
    const auto parsed = details::parse_sources(input, format);

    auto inputs_itr = parsed->second.rbegin();
    auto formats_itr = parsed->first.rbegin();

    details::scan_result<Ts...> result;
    result.values_ = std::make_tuple(
        details::parse_value_with_format<Ts>(
            *(inputs_itr++), 
            *(formats_itr++)
        )...
    );
    return result;
}

} // namespace stdx
