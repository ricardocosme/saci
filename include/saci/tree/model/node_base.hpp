
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "saci/tree/model/detail/T_is_function_obj.hpp"
#include "saci/tree/model/detail/result_of_get_object.hpp"
#include "saci/tree/model/node_base_common.hpp"

#include <type_traits>

namespace saci { namespace tree {

template<typename Object,
         typename CheckPolicy,
         typename ExpandPolicy,
         typename Parent,
         typename EnableIfTIsFunctionObj = void>
struct node_base;

template<typename GetObject,
         typename CheckPolicy,
         typename ExpandPolicy,
         typename Parent>
struct node_base<
    GetObject,
    CheckPolicy,
    ExpandPolicy,
    Parent,
    detail::enable_if_T_is_function_obj<GetObject, Parent>
>
    : node_base_common<
    typename detail::result_of_get_object<GetObject, Parent>,
          CheckPolicy,
          ExpandPolicy,
          Parent>
{
    //TODO I need to handle the case that GetObject returns a
    //ObservableErasableRange and the other that occurs when the
    //function returns an object.
    using type =
        typename detail::result_of_get_object<GetObject, Parent>;
    using base = node_base_common<
        type,
        CheckPolicy,
        ExpandPolicy,
        Parent>;
    using base::base;
    using T = GetObject;
    using get_object = GetObject;
};

template<typename Object,
         typename CheckPolicy,
         typename ExpandPolicy,
         typename Parent>
struct node_base<
    Object,
    CheckPolicy,
    ExpandPolicy,
    Parent,
    detail::enable_if_T_is_not_function_obj<Object, Parent>
>
    : node_base_common<
          Object,
          CheckPolicy,
          ExpandPolicy,
          Parent>
{
    using base = node_base_common<
        Object,
        CheckPolicy,
        ExpandPolicy,
        Parent>;
    using base::base;
    using T = Object;
    using type = Object;
};

}}
