#pragma once

namespace saci { namespace tree { 

template<typename T,
         typename CheckPolicy,
         typename Parent,
         typename EnableIfTIsFuncObj = void>
struct leaves_impl;

template<typename GetCollection,
         typename CheckPolicy,
         typename Parent>
struct leaves_impl<
    GetCollection,
    CheckPolicy,
    Parent,
    detail::enable_if_T_is_function_obj<GetCollection, Parent>
    > : coruja::list<
    node_base<
        typename detail::result_of_get_object<GetCollection, Parent>::value_type,
        CheckPolicy,
        UnExpandable,
        Parent>>
{
    using base = coruja::list<
    node_base<
        typename detail::result_of_get_object<GetCollection, Parent>::value_type,
        CheckPolicy,
        UnExpandable,
        Parent>>;
    using base::base;
    
    using get_collection = GetCollection;
};

template<typename Collection,
         typename CheckPolicy,
         typename Parent>
struct leaves_impl<
    Collection,
    CheckPolicy,
    Parent,
    detail::enable_if_T_is_not_function_obj<Collection, Parent>
    > : coruja::list<
    node_base<
        typename Collection::value_type,
        CheckPolicy,
        UnExpandable,
        Parent>>
{
    using base = coruja::list<
    node_base<
        typename Collection::value_type,
        CheckPolicy,
        UnExpandable,
        Parent>>;
    using base::base;
};

}}
