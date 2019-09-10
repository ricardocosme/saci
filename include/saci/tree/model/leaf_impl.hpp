#pragma once

#include "saci/tree/model/detail/expand.hpp"
#include "saci/tree/model/node_base.hpp"

namespace saci { namespace tree {

template<typename T,
         typename CheckPolicy,
         typename Parent>
using leaf_impl = node_base<T, CheckPolicy, detail::UnExpandable, Parent>;

}}

#include "saci/tree/model/detail/leaf.hpp"
