#pragma once

#include <boost/mpl/vector.hpp>

namespace saci { namespace tree {

namespace detail {
struct tag_branch{};
} //namespace detail

template<typename T,
         typename CheckPolicy,
         typename... Children>
struct branch : detail::tag_branch {
    using type = T;
    using check_t = CheckPolicy;
    using children = boost::mpl::vector<Children...>;
};

}}
