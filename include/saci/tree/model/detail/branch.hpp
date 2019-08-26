#pragma once

#include <boost/mpl/front.hpp>
#include <boost/hof/is_invocable.hpp>

#include "saci/tree/model/detail/node_impl_fwd.hpp"
#include "saci/tree/model/detail/T_is_function_obj.hpp"

namespace saci { namespace tree {

namespace detail {

template<typename P, typename T>
struct node_impl<P, T, typename std::enable_if<
                      std::is_base_of<tag_branch, T>::value>::type> {
    using type = branch_node<
        typename T::type,
        typename T::check_t,
        typename T::children,
        P>;
};

template<typename P, typename T>
struct node_impl<P, T, typename std::enable_if<
                      std::is_base_of<tag_branches, T>::value>::type> {
    using type = coruja::list<
        branch_node<typename T::type,
                    typename T::check_t,
                    typename T::children,
                    P>>;
};

}

}}
