#pragma once

#include "saci/tree/model/detail/expand.hpp"
#include "saci/tree/model/detail/node_impl_fwd.hpp"
#include "saci/tree/model/detail/sync_with_domain.hpp"
#include "saci/tree/model/detail/tag.hpp"
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
        detail::visibility<
            root<ObservableErasableRange, CheckPolicy, Children>, CheckPolicy>>
{
    using base = coruja::observer_class<
        root, detail::visibility<root, CheckPolicy>>;
    using type = typename ObservableErasableRange::type;
    using ctx_t = typename ObservableErasableRange::ctx_t;
    using check_t = CheckPolicy;
    using expand_t = Expandable;
    using children_t = typename detail::node_impl<root, Children>::type;
    using child = typename children_t::value_type;
    
    root() = default;
    root(type& pobj, ctx_t& pctx)
        : base()
        , obj(&pobj)
        , ctx(&pctx)
    { detail::sync_with_domain(*this, children, pobj); }
    
    root(root&& rhs)
        : base(std::move(rhs))
        , obj(rhs.obj)
        , ctx(rhs.ctx)
        , expand(std::move(rhs.expand))
        , children(std::move(rhs.children))
    { detail::update_parent_ptr<root>{*this}(children); }
    
    root& operator=(root&& rhs) {
        base::operator=(std::move(rhs));
        obj = rhs.obj;
        ctx = rhs.ctx;
        expand = std::move(rhs.expand);
        children = std::move(rhs.children);
        detail::update_parent_ptr<root>{*this}(children);
        return *this;
    }
    
    type* obj{nullptr};
    ctx_t* ctx{nullptr};
    coruja::object<bool> expand{coruja::object<bool>{false}};
    children_t children;
};
    
//TODO Contemplar mais de um filho. root reflete um
//collection_branch_node sem pai. Avaliar a versão de root que é
//análoga a branch_node sem pai.(Considerar root como tag e usar node_impl)
template<typename ObservableErasableRange,
         typename CheckPolicy,
         typename Children>
struct root<ObservableErasableRange,
            CheckPolicy,
            Children,
            detail::enable_if_not_ctx<ObservableErasableRange>
> : coruja::observer_class<
        root<ObservableErasableRange, CheckPolicy, Children>,
        detail::visibility<
            root<ObservableErasableRange, CheckPolicy, Children>, CheckPolicy>>
{
    using base = coruja::observer_class<
        root, detail::visibility<root, CheckPolicy>>;
    using type = ObservableErasableRange;
    using ctx_t = void;
    using check_t = CheckPolicy;
    using expand_t = Expandable;
    using children_t = typename detail::node_impl<root, Children>::type;
    using child = typename children_t::value_type;
    
    root() = default;
    root(type& pobj)
        : base()
        , obj(&pobj)
    { sync_with_domain(*this, children, pobj); }
    
    root(root&& rhs)
        : base(std::move(rhs))
        , obj(rhs.obj)
        , expand(std::move(rhs.expand))
        , children(std::move(rhs.children))
    { detail::update_parent_ptr<root>{*this}(children); }
    
    root& operator=(root&& rhs) {
        base::operator=(std::move(rhs));
        obj = rhs.obj;
        expand = std::move(rhs.expand);
        children = std::move(rhs.children);
        detail::update_parent_ptr<root>{*this}(children);
        return *this;
    }
    
    type* obj{nullptr};
    coruja::object<bool> expand{coruja::object<bool>{false}};
    children_t children;
};

}}
