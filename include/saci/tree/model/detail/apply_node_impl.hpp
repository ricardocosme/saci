#pragma once

#include "saci/tree/model/detail/node_impl_fwd.hpp"

namespace saci { namespace tree { namespace detail {

template<typename Parent>
struct apply_node_impl {
    template<typename T>
    struct apply {
        using type = typename detail::node_impl<Parent, T>::type;
    };
};

template<typename Parent>
struct collection_apply_node_impl {
    template<typename T>
    struct apply {
        using type =
            typename std::conditional<
            std::is_same<typename Parent::collection_child, T>::value,
            typename detail::node_impl<Parent, T>::type,
            typename detail::node_impl<Parent, T>::type
            >::type;
    };
};

}}}
