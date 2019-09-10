#pragma once

#include "saci/tree/model/detail/get_branch_impl.hpp"
#include "saci/tree/model/detail/T_is_function_obj.hpp"
#include "saci/tree/model/node_base.hpp"

#include <coruja/container/list.hpp>

namespace saci { namespace tree { 

template<typename T,
         typename CheckPolicy,
         typename Children,
         typename Parent,
         typename EnableIfTIsFuncObj = void>
struct branches_impl;

template<typename GetCollection,
         typename CheckPolicy,
         typename Children,
         typename Parent>
struct branches_impl<
    GetCollection,
    CheckPolicy,
    Children,
    Parent,
    detail::enable_if_T_is_function_obj<GetCollection, Parent>
    > : coruja::list<
    typename detail::legacy::get_branch_impl<
        typename detail::result_of_get_object<GetCollection, Parent>::value_type,
        CheckPolicy,
        Children,
        Parent>::type
    >
{
    using base = coruja::list<
    typename detail::legacy::get_branch_impl<
        typename detail::result_of_get_object<GetCollection, Parent>::value_type,
        CheckPolicy,
        Children,
        Parent>::type
        >;
    using base::base;
    
    using get_collection = GetCollection;

    using type = GetCollection;
    using check_t = CheckPolicy;
    using parent_t = Parent;
};

template<typename Collection,
         typename CheckPolicy,
         typename Children,
         typename Parent>
struct branches_impl<
    Collection,
    CheckPolicy,
    Children,
    Parent,
    detail::enable_if_T_is_not_function_obj<Collection, Parent>
    > : coruja::list<
    typename detail::legacy::get_branch_impl<
        typename Collection::value_type,
        CheckPolicy,
        Children,
        Parent>::type
    >
{
    using base = coruja::list<
    typename detail::legacy::get_branch_impl<
        typename Collection::value_type,
        CheckPolicy,
        Children,
        Parent>::type
        >;
    using base::base;

    using type = Collection;
    using check_t = CheckPolicy;
    using parent_t = Parent;
};

}}
