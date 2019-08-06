#pragma once

#include <boost/mpl/front.hpp>
#include <boost/hof/is_invocable.hpp>

#include "saci/tree/model/detail/node_impl_fwd.hpp"
#include "saci/tree/model/detail/T_is_function_obj.hpp"

namespace saci { namespace tree {

template<typename Parent, typename T, typename Check = void>
struct get_branch_node;

template<typename Parent, typename T>
struct get_branch_node<
    Parent, T,
    detail::enable_if_T_is_not_function_obj<typename T::type, Parent>
>
{
    using type = branch_node<Parent,
                             typename T::check_t,
                             typename T::type,
                             typename T::children>;
};

template<typename Parent, typename T>
struct get_branch_node<
    Parent, T,
    detail::enable_if_T_is_function_obj<typename T::type, Parent>
>
{
    using type = collection_branch_node<
        Parent,
        typename T::check_t,
        typename T::type,
        typename T::children>;
};

namespace detail {

template<typename P, typename T>
struct node_impl<P, T, typename std::enable_if<
                      std::is_base_of<tag_branch, T>::value>::type> {
    using type = typename get_branch_node<P, T>::type;
};

template<typename P, typename T>
struct node_impl<P, T, typename std::enable_if<
                      std::is_base_of<tag_branches, T>::value>::type> {
    using type = coruja::list<typename get_branch_node<P, T>::type>;
};

}

}}
