#pragma once

#include "saci/tree/model/detail/apply_node_impl.hpp"
#include "saci/tree/model/detail/node_impl_fwd.hpp"
#include "saci/tree/model/detail/sync_with_domain.hpp"
#include "saci/tree/model/detail/tag.hpp"
#include "saci/tree/model/detail/update_parent_ptr.hpp"
#include "saci/tree/model/detail/visibility.hpp"
#include "saci/tree/model/node_base.hpp"

#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <coruja/observer_class.hpp>

#include <type_traits>

namespace saci { namespace tree {

template<typename T, typename CheckPolicy, typename...Children>
struct branch : detail::tag_branch {
    using type = T;
    using check_t = CheckPolicy;
    using children = boost::mpl::vector<Children...>;
};

template<typename ObservableErasableRange, typename CheckPolicy, typename...Children>
struct branches : detail::tag_branches {
    using type = typename ObservableErasableRange::value_type;
    using check_t = CheckPolicy;
    using children = boost::mpl::vector<Children...>;
};

template<typename T,
         typename CheckPolicy,
         typename Children,
         typename Parent,
         typename EnableIfChildren = void>
struct branch_node;

template<typename T,
         typename CheckPolicy,
         typename Children,
         typename Parent>
struct branch_node<
    T,
    CheckPolicy,
    Children,
    Parent,
    typename std::enable_if<boost::mpl::size<Children>::value >= 2>::type
> : coruja::observer_class<
        branch_node<T, CheckPolicy, Children, Parent>,
        node_base<T, CheckPolicy, Expandable, Parent>
    >
{
    using base = coruja::observer_class<
        branch_node,
        node_base<T, CheckPolicy, Expandable, Parent>>;
    
    using ctx_t = void;
    
    using children_t = typename boost::fusion::result_of::as_vector<
        typename boost::mpl::transform<
            Children, detail::apply_node_impl<branch_node>>::type
    >::type;
    
    branch_node() = default;
    
    branch_node(typename base::type& o, Parent& p) : base(o, p)
    {
        boost::fusion::for_each(children, detail::sync_with_domain_t<branch_node>{*this});
    }
    
    branch_node(branch_node&&) = delete;
    
    branch_node& operator=(branch_node&& rhs) {
        base::operator=(std::move(rhs));
        children = std::move(rhs.children);
        boost::fusion::for_each
            (children, detail::update_parent_ptr<branch_node>{*this});
        return *this;
    }
    
    void update_parent_ptr(Parent& p) {
        base::update_parent_ptr(p);
        boost::fusion::for_each
            (children, detail::update_parent_ptr<branch_node>{*this});
    }
    
    children_t children;
};

template<typename T,
         typename CheckPolicy,
         typename Children,
         typename Parent>
struct branch_node<
    T,
    CheckPolicy,
    Children,
    Parent,
    typename std::enable_if<boost::mpl::size<Children>::value == 1>::type
> : coruja::observer_class<
        branch_node<T, CheckPolicy, Children, Parent>,
        node_base<T, CheckPolicy, Expandable, Parent>
    >
{
    using base = coruja::observer_class<
        branch_node,
        node_base<T, CheckPolicy, Expandable, Parent>>;
    
    using ctx_t = void;

    using children_t = typename detail::node_impl<
        branch_node, typename boost::mpl::front<Children>::type
    >::type;
    
    branch_node() = default;
    
    branch_node(typename base::type& o, Parent& p) : base(o, p)
    {
        detail::sync_with_domain_t<branch_node>{*this}(children);
    }
    
    branch_node(branch_node&&) = delete;
    
    branch_node& operator=(branch_node&& rhs) {
        base::operator=(std::move(rhs));
        children = std::move(rhs.children);
        detail::update_parent_ptr<branch_node>{*this}(children);
        return *this;
    }
    
    void update_parent_ptr(Parent& p) {
        base::update_parent_ptr(p);
        detail::update_parent_ptr<branch_node>{*this}(children);
    }
    
    children_t children;
};

}}

#include "saci/tree/model/detail/branch.hpp"
