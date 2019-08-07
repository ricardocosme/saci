
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

namespace saci { namespace tree { namespace detail {

template<typename GetObject, typename Parent>
using result_of_get_object =
    typename std::remove_reference<
        typename std::result_of<
            GetObject(typename Parent::type&)
                >::type
    >::type;

}}}
