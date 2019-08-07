
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "saci/tree/model/detail/expand.hpp"
#include "saci/tree/model/detail/visibility.hpp"

namespace saci { namespace tree {

template<typename Object,
         typename CheckPolicy,
         typename ExpandPolicy,
         typename Parent>
struct node_base_common
    : detail::visibility<
          node_base_common<Object, CheckPolicy, ExpandPolicy, Parent>,
          CheckPolicy,
          Parent>
    , detail::expand<ExpandPolicy>
{
    using visibility_base = detail::visibility<
        node_base_common, CheckPolicy, Parent>;
    using expand_base = detail::expand<ExpandPolicy>;

    using check_t = CheckPolicy;
    using expand_t = ExpandPolicy;
    using parent_t = Parent;
    
    node_base_common() = default;
    
    node_base_common(Object& _obj, parent_t& _parent)
        : visibility_base(_parent)
        , expand_base()
        , parent(&_parent)
        , obj(&_obj)
    {}

    void update_parent_ptr(Parent& o) {
        parent = &o;
        static_cast<visibility_base&>(*this).update_parent_ptr(o);
    }
    
    parent_t* parent{nullptr};
    Object* obj{nullptr};
};

}}
