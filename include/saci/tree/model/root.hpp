
// Copyright Ricardo Calheiros de Miranda Cosme 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "saci/tree/model/detail/expand.hpp"
#include "saci/tree/model/detail/node_impl_fwd.hpp"
#include "saci/tree/model/detail/sync_with_domain.hpp"
#include "saci/tree/model/detail/update_parent_ptr.hpp"
#include "saci/tree/model/detail/with_ctx_base.hpp"
#include "saci/tree/model/node_base.hpp"

#include <coruja/observer_class.hpp>

#include <utility>

namespace saci { namespace tree {

template<typename ObservableErasableRange,
         typename CheckPolicy,
         typename Children,
         typename EnableIfHasCtx = void>
struct root;

template<typename ObservableErasableRange,
         typename CheckPolicy,
         typename Children>
struct root<ObservableErasableRange,
            CheckPolicy,
            Children,
            detail::enable_if_not_ctx<ObservableErasableRange>
> : coruja::observer_class<
        root<ObservableErasableRange, CheckPolicy, Children>,
        node_base<
            ObservableErasableRange,
            CheckPolicy,
            detail::Expandable>
    >
{
    using base = coruja::observer_class<
        root,
        node_base<
            ObservableErasableRange,
            CheckPolicy,
            detail::Expandable>
        >;
    using ctx_t = void;
    using children_t = typename detail::node_impl<root, Children>::type;
    using child = typename children_t::value_type;
    
    root() = default;
    explicit root(typename base::type& model)
        : base(model)
    { detail::sync_with_domain<children_t>(*this, model); }
    
    root(root&& rhs)
        : base(std::move(rhs))
        , children(std::move(rhs.children))
    { detail::update_parent_ptr<root>{*this}(children); }
    
    root& operator=(root&& rhs) {
        base::operator=(std::move(rhs));
        children = std::move(rhs.children);
        detail::update_parent_ptr<root>{*this}(children);
        return *this;
    }
    
    children_t children;
};

template<typename ObservableErasableRange,
         typename CheckPolicy,
         typename Children>
struct root<ObservableErasableRange,
            CheckPolicy,
            Children,
            detail::enable_if_ctx<ObservableErasableRange>
> : coruja::observer_class<
        root<ObservableErasableRange, CheckPolicy, Children>,
        node_base<typename ObservableErasableRange::type,
                  CheckPolicy,
                  detail::Expandable>
    >
{
    using base = coruja::observer_class<
        root,
        node_base<typename ObservableErasableRange::type,
                  CheckPolicy,
                  detail::Expandable>
        >;
    
    using ctx_t = typename ObservableErasableRange::ctx_t;
    using children_t = typename detail::node_impl<root, Children>::type;
    using child = typename children_t::value_type;
    
    root() = default;
    explicit root(typename base::type& model, ctx_t& pctx)
        : base(model)
        , ctx(&pctx)
    { detail::sync_with_domain<children_t>(*this, model); }
    
    root(root&& rhs)
        : base(std::move(rhs))
        , ctx(rhs.ctx)
        , children(std::move(rhs.children))
    { detail::update_parent_ptr<root>{*this}(children); }
    
    root& operator=(root&& rhs) {
        base::operator=(std::move(rhs));
        ctx = rhs.ctx;
        children = std::move(rhs.children);
        detail::update_parent_ptr<root>{*this}(children);
        return *this;
    }
    
    ctx_t* ctx{nullptr};
    children_t children;
};
    
}}
