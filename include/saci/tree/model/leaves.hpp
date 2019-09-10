#pragma once

namespace saci { namespace tree {

namespace detail {
struct tag_leaves{};
} //namespace detail

template<typename T,
         typename CheckPolicy>
struct leaves : detail::tag_leaves {
    using type = T;
    using check_t = CheckPolicy;
};

}}
