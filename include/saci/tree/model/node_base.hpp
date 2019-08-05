
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "saci/tree/model/detail/expand.hpp"
#include "saci/tree/model/detail/visibility.hpp"

namespace saci { namespace tree {

template<typename T,
         typename CheckPolicy,
         typename ExpandPolicy,
         typename Parent>
struct node_base
    : detail::visibility<node_base<T, CheckPolicy, ExpandPolicy, Parent>,
                         CheckPolicy,
                         Parent>
    , detail::expand<ExpandPolicy>
{
    using visibility_base = detail::visibility<node_base, CheckPolicy, Parent>;
    using expand_base = detail::expand<ExpandPolicy>;

    using type = T;
    using check_t = CheckPolicy;
    using expand_t = ExpandPolicy;
    using parent_t = Parent;
    
    node_base() = default;
    
    node_base(type& pobj, parent_t& p)
        : visibility_base(p)
        , expand_base()
        , parent(&p)
        , obj(&pobj)
    {}

    void update_parent_ptr(Parent& p) {
        parent = &p;
        static_cast<visibility_base&>(*this).update_parent_ptr(p);
    }
    
    parent_t* parent{nullptr};
    type* obj{nullptr};
};

}}
