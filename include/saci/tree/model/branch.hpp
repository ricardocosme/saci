#pragma once

#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <coruja/container/list.hpp>
#include <coruja/object/any_object_view.hpp>
#include <coruja/object/lift.hpp>
#include <coruja/object/object.hpp>
#include <coruja/observer_class.hpp>
#include <coruja/support/type_traits.hpp>

#include "saci/tree/model/branch_base.hpp"
#include "saci/tree/model/detail/apply_node_impl.hpp"
#include "saci/tree/model/detail/update_parent_ptr.hpp"
#include "saci/tree/model/detail/init_children.hpp"
#include "saci/tree/model/detail/is_collection_leaf.hpp"
#include "saci/tree/model/detail/node_impl_fwd.hpp"
#include "saci/tree/model/detail/sync_with_domain.hpp"
#include "saci/tree/model/detail/tag.hpp"
#include "saci/tree/model/detail/visibility.hpp"

#include <type_traits>

namespace saci { namespace tree {

template<typename T, typename CheckPolicy, typename...Children>
struct branch : detail::tag_branch {
    using type = T;
    using check_t = CheckPolicy;
    using children = boost::mpl::vector<Children...>;
};

template<typename T, typename CheckPolicy, typename...Children>
struct branches : detail::tag_branches {
    using type = T;
    using check_t = CheckPolicy;
    using children = boost::mpl::vector<Children...>;
};

struct child_of_base {};

template<typename Collection, typename Child>
struct child_of : child_of_base {
    using collection = Collection;
    using child = Child;
};

template<typename Parent,
         typename CheckPolicy,
         typename T,
         typename Children>
struct branch_node : branch_base<Parent, CheckPolicy, Expandable, T>
{
    using base = branch_base<Parent, CheckPolicy, Expandable, T>;

    using children_t = typename boost::fusion::result_of::as_vector<
        typename boost::mpl::transform<
            Children, detail::apply_node_impl<branch_node>>::type
    >::type;
    
    branch_node() = default;
    branch_node(Parent& p, T& o) : base(p, o)
    {boost::fusion::for_each(children, detail::init_children<branch_node>{*this});}
    
    branch_node(branch_node&&) = delete;
    branch_node& operator=(branch_node&&) = delete;
    
    void update_parent_ptr(Parent& p) {
        base::update_parent_ptr(p);
        boost::fusion::for_each
            (children, detail::update_parent_ptr<branch_node>{*this});
    }
    
    children_t children;
};

template<typename Parent,
         typename CheckPolicy,
         typename GetContainer,
         typename Child
>
struct collection_branch_node_impl
    : coruja::observer_class<
    collection_branch_node_impl<Parent,CheckPolicy, GetContainer, Child>,
    branch_base<Parent, CheckPolicy, Expandable,
                typename std::remove_reference<
                    typename std::result_of<
                        GetContainer(typename Parent::type&)
                            >::type
                    >::type>>
{
    using T = typename std::remove_reference<
        typename std::result_of<
            GetContainer(typename Parent::type&)
                >::type
        >::type;

    using base = coruja::observer_class<
        collection_branch_node_impl<Parent, CheckPolicy, GetContainer, Child>,
        branch_base<Parent, CheckPolicy, Expandable, T>>;

    using check_t = CheckPolicy;
    using children_t = typename detail::node_impl<
        collection_branch_node_impl, Child>::type;
    using child = typename children_t::value_type;

    using get_container = GetContainer;
    
    collection_branch_node_impl() = default;
    collection_branch_node_impl(Parent& p, T& pobj) : base(p, pobj)
    {
        sync_with_domain(*this, pobj);
    }
    
    collection_branch_node_impl(collection_branch_node_impl&&) = delete;
    collection_branch_node_impl& operator=(collection_branch_node_impl&&) = default;
    
    void update_parent_ptr(Parent& p) {
        base::update_parent_ptr(p);
        detail::update_parent_ptr<collection_branch_node_impl>
        {*this}(children);
    }
    
    children_t children;
};

template<typename Parent,
         typename CheckPolicy,
         typename GetContainer,
         typename Children
>
struct collection_branch_node_children_impl
    : coruja::observer_class<
    collection_branch_node_children_impl<Parent,CheckPolicy, GetContainer, Children>,
    branch_base<Parent, CheckPolicy, Expandable,
                typename std::remove_reference<
                    typename std::result_of<
                        GetContainer(typename Parent::type&)
                            >::type
                    >::type>>
{
    using T = typename std::remove_reference<
        typename std::result_of<
            GetContainer(typename Parent::type&)
                >::type
        >::type;

    using collection_child = typename boost::mpl::deref<
        typename boost::mpl::find_if<
            Children, is_collection_leaf<typename T::value_type>>::type>::type;
    
    using base = coruja::observer_class<
        collection_branch_node_children_impl<Parent, CheckPolicy, GetContainer, Children>,
        branch_base<Parent, CheckPolicy, Expandable, T>>;

    using check_t = CheckPolicy;

    using children_t = typename boost::fusion::result_of::as_vector<
        typename boost::mpl::transform<
            Children, detail::apply_node_impl<collection_branch_node_children_impl>>::type
    >::type;
    using get_container = GetContainer;
    
    collection_branch_node_children_impl() = default;
    collection_branch_node_children_impl(Parent& p, T& pobj) : base(p, pobj)
    {
        boost::fusion::for_each
            (children,
             detail::sync_with_domain_t<collection_branch_node_children_impl>{*this});
    }
    
    collection_branch_node_children_impl(collection_branch_node_children_impl&&) = delete;
    collection_branch_node_children_impl& operator=(collection_branch_node_children_impl&& rhs) {
        base::operator=(std::move(rhs));
        children = std::move(rhs.children);
        boost::fusion::for_each
            (children,
             detail::assign_sync_with_domain_t<collection_branch_node_children_impl>{*this});
        return *this;
    }
    
    void update_parent_ptr(Parent& p) {
        base::update_parent_ptr(p);
        boost::fusion::for_each
            (children, detail::update_parent_ptr
             <collection_branch_node_children_impl>{*this});
    }
    
    children_t children;
};

template<typename Parent,
         typename CheckPolicy,
         typename GetContainer,
         typename Children,
         typename Check = void
>
struct collection_branch_node;

template<typename Parent,
         typename CheckPolicy,
         typename GetContainer,
         typename Children
>
struct collection_branch_node<
    Parent,
    CheckPolicy,
    GetContainer,
    Children,
    typename std::enable_if<boost::mpl::size<Children>::value == 1>::type
>
    : collection_branch_node_impl<Parent, CheckPolicy, GetContainer,
                                  typename boost::mpl::front<Children>::type>
{
    using base = collection_branch_node_impl<
        Parent, CheckPolicy, GetContainer,
        typename boost::mpl::front<Children>::type>;
    using base::base;
};

template<typename Parent,
         typename CheckPolicy,
         typename GetContainer,
         typename Children
>
struct collection_branch_node<
    Parent,
    CheckPolicy,
    GetContainer,
    Children,
    typename std::enable_if<!(boost::mpl::size<Children>::value == 1)>::type
>
    : collection_branch_node_children_impl<Parent, CheckPolicy, GetContainer, Children>
{
    using base = collection_branch_node_children_impl<
        Parent, CheckPolicy, GetContainer, Children>;
    using base::base;
};

}}

#include "saci/tree/model/detail/branch.hpp"
