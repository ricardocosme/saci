#pragma once

namespace saci { namespace tree {

namespace detail {
struct tag_leaf{};
} //namespace detail

template<typename T, typename CheckPolicy>
struct leaf : detail::tag_leaf {
    using type = T;
    using check_t = CheckPolicy;
};

}}
