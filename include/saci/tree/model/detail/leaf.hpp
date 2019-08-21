#pragma once

#include "saci/tree/model/detail/node_impl_fwd.hpp"
#include "saci/tree/model/leaves_impl.hpp"

#include <type_traits>

namespace saci { namespace tree { namespace detail {

template<typename Parent, typename T>
struct node_impl<Parent, T,
               typename std::enable_if<
                   std::is_base_of<tag_leaf, T>::value>::type>
{ using type = leaf_node<typename T::type, typename T::check_t, Parent>; };

template<typename Parent, typename T>
struct node_impl<Parent, T,
               typename std::enable_if<
                   std::is_base_of<tag_leaves, T>::value>::type>
{
    using type = leaves_impl<typename T::type, typename T::check_t, Parent>;
};

}}}
