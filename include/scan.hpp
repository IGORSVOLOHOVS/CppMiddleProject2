#pragma once

#include "parse.hpp"
#include "types.hpp"
#include <tuple>
#include <concepts>

namespace stdx {

// замените болванку функции scan на рабочую версию
template <typename... Ts>
std::expected<details::scan_result<Ts...>, details::scan_error> scan(std::string_view input, std::string_view format) {
    auto parsed = details::parse_sources(input, format);
    
    size_t input_indx = {0}, format_indx = {0}; 
    details::scan_result<Ts...> result = std::make_tuple((details::parse_value_with_format<Ts>(parsed->first.at(input_indx++), parsed->second.at(format_indx++)), ...));

    if constexpr ((std::same_as<Ts, details::scan_error> || ...)){
        return std::unexpected(details::scan_error{"Dumb implementation"});
    }else{
        return result;
    }
}

} // namespace stdx
