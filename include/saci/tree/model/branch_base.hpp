#pragma once

#include <coruja/object/object.hpp>

#include "saci/tree/model/detail/expand.hpp"
#include "saci/tree/model/detail/visibility.hpp"

namespace saci { namespace tree {

template<typename Parent,
         typename CheckPolicy,
         typename ExpandPolicy,
         typename T>
struct branch_base
    : detail::visibility<branch_base<Parent, CheckPolicy, ExpandPolicy, T>,
                         CheckPolicy, Parent>
    , detail::expand<ExpandPolicy>
{
    using visibility_base = detail::visibility<branch_base, CheckPolicy, Parent>;
    using expand_base = detail::expand<ExpandPolicy>;

    using parent_t = Parent;
    using check_t = CheckPolicy;
    using expand_t = ExpandPolicy;
    using type = T;
    
    branch_base() = default;
    branch_base(parent_t& p, type& pobj)
        : visibility_base(p)
        , expand_base()
        , parent(&p)
        , obj(&pobj)
    {}

    branch_base(branch_base&& rhs)
        : visibility_base(std::move(rhs))
        , expand_base(std::move(rhs))
        , obj(rhs.obj)
    {}
    
    branch_base& operator=(branch_base&& rhs) {
        visibility_base::operator=(std::move(rhs));
        expand_base::operator=(std::move(rhs));
        obj = rhs.obj;
        return *this;
    }
    
    void update_parent_ptr(Parent& p) {
        parent = &p;
        static_cast<visibility_base&>(*this).update_parent_ptr(p);
    }
    
    parent_t* parent{nullptr};
    type* obj{nullptr};
};

}}
