
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

namespace saci { namespace tree { namespace detail {

struct with_ctx_base {};

template<typename T>
using enable_if_ctx = typename std::enable_if<
    std::is_base_of<detail::with_ctx_base, T>::value
>::type;

template<typename T>
using enable_if_not_ctx = typename std::enable_if<
    !std::is_base_of<detail::with_ctx_base, T>::value
>::type;

}}}
