#pragma once

#include "saci/tree/model/detail/node_impl_fwd.hpp"

#include <type_traits>

namespace saci { namespace tree { namespace detail {

template<typename Parent, typename T>
struct node_impl<Parent, T,
               typename std::enable_if<
                   std::is_base_of<tag_leaf, T>::value>::type>
{ using type = leaf_node<Parent, typename T::check_t, typename T::type>; };

template<typename Parent, typename T>
struct node_impl<Parent, T,
               typename std::enable_if<
                   std::is_base_of<tag_leaves, T>::value>::type>
{
    using type = coruja::list<
        leaf_node<Parent, typename T::check_t, typename T::type>>;
};

}}}
