
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "saci/tree/model/detail/with_ctx_base.hpp"

namespace saci { namespace tree {

template<typename T, typename Context>
struct with_ctx : detail::with_ctx_base {
    using type = T;
    using ctx_t = Context;
};

}}
