#pragma once

#include "saci/tree/model/detail/expand.hpp"
#include "saci/tree/model/detail/node_impl_fwd.hpp"
#include "saci/tree/model/detail/sync_with_domain.hpp"
#include "saci/tree/model/detail/tag.hpp"
#include "saci/tree/model/detail/update_parent_ptr.hpp"
#include "saci/tree/model/detail/visibility.hpp"
#include "saci/tree/model/detail/with_ctx_base.hpp"

#include <coruja/container/list.hpp>
#include <coruja/object/object.hpp>
#include <coruja/observer_class.hpp>

#include <type_traits>

namespace saci { namespace tree {

template<typename T, typename Context>
struct with_ctx : detail::with_ctx_base {
    using type = T;
    using ctx_t = Context;
};

template<typename T, typename CheckPolicy, typename Child,
         typename Enable = void>
struct root;

template<typename T, typename CheckPolicy, typename Child>
struct root<T, CheckPolicy, Child, detail::enable_if_ctx<T>>
    : coruja::observer_class<root<T, CheckPolicy, Child>,
                             detail::visibility<root<T, CheckPolicy, Child>,
                                                CheckPolicy>>
{
    using base = coruja::observer_class<root, detail::visibility<root, CheckPolicy>>;
    using type = typename T::type;
    using ctx_t = typename T::ctx_t;
    using check_t = CheckPolicy;
    using expand_t = Expandable;
    using children_t = typename detail::node_impl<root, Child>::type;
    using child = typename children_t::value_type;
    
    root() = default;
    root(type& pobj, ctx_t& pctx)
        : base()
        , obj(&pobj)
        , ctx(&pctx)
    { sync_with_domain(*this, pobj); }
    
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
template<typename T, typename CheckPolicy, typename Child>
struct root<T, CheckPolicy, Child, detail::enable_if_not_ctx<T>>
    : coruja::observer_class<root<T, CheckPolicy, Child>,
                             detail::visibility<root<T, CheckPolicy, Child>,
                                                CheckPolicy>>
{
    using base = coruja::observer_class<root, detail::visibility<root, CheckPolicy>>;
    using type = T;
    using ctx_t = void;
    using check_t = CheckPolicy;
    using expand_t = Expandable;
    using children_t = typename detail::node_impl<root, Child>::type;
    using child = typename children_t::value_type;
    
    root() = default;
    root(type& pobj)
        : base()
        , obj(&pobj)
    { sync_with_domain(*this, pobj); }
    
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
