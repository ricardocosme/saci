#pragma once

#include <boost/mpl/vector.hpp>

namespace saci { namespace tree {

namespace detail {
struct tag_branches{};
} //namespace detail

template<typename T,
         typename CheckPolicy,
         typename... Children>
struct branches : detail::tag_branches {
    using type = T;
    using check_t = CheckPolicy;
    using children = boost::mpl::vector<Children...>;
};

}}
