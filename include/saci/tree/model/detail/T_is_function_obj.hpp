
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/hof/is_invocable.hpp>

#include <type_traits>

namespace saci { namespace tree { namespace detail {

template<typename T, typename Parent>
using enable_if_T_is_function_obj = typename std::enable_if<
    boost::hof::is_invocable<
        T,
        typename Parent::type&
        >::value
    >::type;

template<typename T, typename Parent>
using enable_if_T_is_not_function_obj = typename std::enable_if<
    !boost::hof::is_invocable<
        T,
        typename Parent::type&
        >::value
    >::type;

}}}
