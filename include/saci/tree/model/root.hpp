#pragma once

#include "saci/tree/model/node_base.hpp"
#include "saci/tree/model/detail/expand.hpp"
#include "saci/tree/model/detail/node_impl_fwd.hpp"
#include "saci/tree/model/detail/sync_with_domain.hpp"
#include "saci/tree/model/detail/update_parent_ptr.hpp"
#include "saci/tree/model/detail/visibility.hpp"
#include "saci/tree/model/with_ctx.hpp"

#include <coruja/container/list.hpp>
#include <coruja/object/object.hpp>
#include <coruja/observer_class.hpp>

#include <type_traits>

namespace saci { namespace tree {

template<typename ObservableErasableRange,
         typename CheckPolicy,
         typename Children,
         typename Enable = void>
struct root;

template<typename ObservableErasableRange,
         typename CheckPolicy,
         typename Children>
struct root<ObservableErasableRange,
            CheckPolicy,
            Children,
            detail::enable_if_ctx<ObservableErasableRange>
> : coruja::observer_class<
        root<ObservableErasableRange, CheckPolicy, Children>,
        node_base<typename ObservableErasableRange::type, CheckPolicy, Expandable, dummy_parent>
    >
{
    using base = coruja::observer_class<
        root,
        node_base<typename ObservableErasableRange::type, CheckPolicy, Expandable, dummy_parent>
        >;
    
    using ctx_t = typename ObservableErasableRange::ctx_t;
    using children_t = typename detail::node_impl<root, Children>::type;
    using child = typename children_t::value_type;
    
    root() = default;
    explicit root(typename base::type& pobj, ctx_t& pctx)
        : base(pobj)
        , ctx(&pctx)
    { detail::sync_with_domain<children_t>(*this, pobj); }
    
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
    
//TODO Contemplar mais de um filho. root reflete um
//collection_branch_node sem pai. Avaliar a versão de root que é
//análoga a branch_impl sem pai.(Considerar root como tag e usar node_impl)
template<typename ObservableErasableRange,
         typename CheckPolicy,
         typename Children>
struct root<ObservableErasableRange,
            CheckPolicy,
            Children,
            detail::enable_if_not_ctx<ObservableErasableRange>
> : coruja::observer_class<
        root<ObservableErasableRange, CheckPolicy, Children>,
        node_base<ObservableErasableRange, CheckPolicy, Expandable, dummy_parent>
    >
{
    using base = coruja::observer_class<
        root,
        node_base<ObservableErasableRange, CheckPolicy, Expandable, dummy_parent>
        >;
    using ctx_t = void;
    using children_t = typename detail::node_impl<root, Children>::type;
    using child = typename children_t::value_type;
    
    root() = default;
    explicit root(typename base::type& pobj)
        : base(pobj)
    { detail::sync_with_domain<children_t>(*this, pobj); }
    
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

}}
