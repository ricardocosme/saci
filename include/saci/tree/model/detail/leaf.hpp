#pragma once

#include "saci/tree/model/detail/node_impl_fwd.hpp"
#include "saci/tree/model/leaf.hpp"
#include "saci/tree/model/leaves.hpp"
#include "saci/tree/model/leaves_impl.hpp"

#include <type_traits>

namespace saci { namespace tree { namespace detail {

template<typename Parent, typename Tag>
struct node_impl<Parent, Tag,
               typename std::enable_if<
                   std::is_base_of<tag_leaf, Tag>::value>::type>
{
    using type = leaf_impl<
        typename Tag::type,
        typename Tag::check_t,
        Parent>;
};

template<typename Parent, typename Tag>
struct node_impl<Parent, Tag,
               typename std::enable_if<
                   std::is_base_of<tag_leaves, Tag>::value>::type>
{
    using type = leaves_impl<
        typename Tag::type,
        typename Tag::check_t,
        Parent>;
};

}}}
