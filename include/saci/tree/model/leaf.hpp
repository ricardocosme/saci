#pragma once

#include "saci/tree/model/detail/expand.hpp"
#include "saci/tree/model/detail/tag.hpp"
#include "saci/tree/model/node_base.hpp"

namespace saci { namespace tree {

template<typename T, typename CheckPolicy>
struct leaf : detail::tag_leaf {
    using type = T;
    using check_t = CheckPolicy;
};

template<typename T, typename CheckPolicy>
struct leaves : detail::tag_leaves {
    using type = T;
    using check_t = CheckPolicy;
};

template<typename T,
         typename CheckPolicy,
         typename Parent>
using leaf_node = node_base<T, CheckPolicy, UnExpandable, Parent>;

}}

#include "saci/tree/model/detail/leaf.hpp"
