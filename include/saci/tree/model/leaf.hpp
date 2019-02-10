#pragma once

#include <coruja/object/any_object_view.hpp>
#include <coruja/object/lift.hpp>
#include <coruja/object/object.hpp>

#include "saci/tree/model/detail/tag.hpp"
#include "saci/tree/model/detail/expand.hpp"
#include "saci/tree/model/detail/visibility.hpp"
#include "saci/tree/model/detail/node_impl_fwd.hpp"

#include <type_traits>

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

template<typename Parent,
         typename CheckPolicy,
         typename T>
using leaf_node = branch_base<Parent, CheckPolicy, UnExpandable, T>;

}}

#include "saci/tree/model/detail/leaf.hpp"
